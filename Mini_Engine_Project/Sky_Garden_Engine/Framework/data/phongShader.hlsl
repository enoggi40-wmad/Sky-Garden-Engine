#define NUM_POINT_LIGHTS 2


cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float padding;
};

cbuffer PointLightPositionBuffer : register(b2)
{
    float4 pointLightPosition[NUM_POINT_LIGHTS];
};

cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

cbuffer PointLightColorBuffer : register(b1)
{
    float4 pointLightColor[NUM_POINT_LIGHTS];
};


Texture2D shaderTexture;
SamplerState SampleType;


struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 pointLightPos1 : TEXCOORD2;
    float3 pointLightPos2 : TEXCOORD3;
};


// Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;


	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3) worldMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);

	// Determine the point light positions based on the position of the lights and the position of the vertex in the world.
    output.pointLightPos1 = pointLightPosition[0].xyz - worldPosition.xyz;
    output.pointLightPos2 = pointLightPosition[1].xyz - worldPosition.xyz;

	// Normalize the point light position vectors.
    output.pointLightPos1 = normalize(output.pointLightPos1);
    output.pointLightPos2 = normalize(output.pointLightPos2);

    return output;
}


// Pixel Shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;

    float pointLightIntensity1;
    float pointLightIntensity2;
    float4 pointLightColor1;
    float4 pointLightColor2;


	// Sample the texture pixel at this location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	// Initialize the specular light color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

	// Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
		// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += diffuseColor * lightIntensity;

		// Saturate the light color.
        color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = specularColor * pow(
			saturate(dot(reflection, input.viewDirection)),
			specularPower
		);
    }


	// Calculate the amount of light on this pixel from the first point light.
    pointLightIntensity1 = saturate(dot(input.normal, input.pointLightPos1));

	// Calculate the amount of light on this pixel from the second point light.
    pointLightIntensity2 = saturate(dot(input.normal, input.pointLightPos2));

	// Determine the diffuse color amount of each point light.
    pointLightColor1 = pointLightColor[0] * pointLightIntensity1;
    pointLightColor2 = pointLightColor[1] * pointLightIntensity2;


	// Multiply the texture pixel by the lighting.
    color = saturate(color + pointLightColor1 + pointLightColor2);
    color = color * textureColor;

	// Add the specular component.
    color = saturate(color + specular);

    return color;
}