// Test shader for engine
// 				by snakEZ

Texture2D TextureColor: register(t0);
sampler TextureColorSampler: register(s0);

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_light_direction;
	float4 m_camera_position;
    float4 m_light_position;
    float m_light_radius;
    float m_time = 0;
};

//
// Vertex shader section
//

struct VS_INPUT
{
	float4 position:	POSITION0;
	float2 texcoord:	TEXCOORD0;
	float3 normal:		TEXCOORD1;
};

struct VS_OUTPUT
{
	float4 position:			SV_POSITION;
	float2 texcoord:			TEXCOORD0;
    float3 normal:				NORMAL1;
    float3 world_pos:			TEXCOORD1;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//	output.position = lerp(input.position, input.position1, (float)((sin((float)(m_time / (float)1000.0f)) + 1.0f) / 2.0f));

	//WORLD SPACE
	output.position = mul(input.position, m_world);
	
	//VIEW SPACE
	output.position = mul(output.position, m_view);
	
	//SCREEN SPACE
	output.position = mul(output.position, m_proj);

	output.texcoord = input.texcoord;
	output.normal = input.normal;
	return output;
}

//
// Pixel shader section
//

struct PS_INPUT
{
	float4 position:			SV_POSITION;
	float2 texcoord:			TEXCOORD0;
    float3 normal:				NORMAL0;
    float3 world_pos:			TEXCOORD1;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 material = Material.Sample(MaterialSampler, 1.0 - input.texcoord);
	
	// Ambient lighting section
	float ka = 0.1;						//Ambient reflection coefficient
	float3 ia = float3(1.0, 1.0, 1.0);	//Ambient light color
	ia *= material.rgb;
	
	float ambient_light = ka * ia;	//Ambient lighting
	
	
	// Diffuse lighting section
	float kd = 0.7;						//Diffuse reflection coefficient
	
    float3 light_dir = normalize(m_light_position.xyz - input.world_pos.xyz);
	
    float amount_diffuse_light = max(0, dot(light_dir.xyz, input.normal)); // dot product
	float3 id = float3(1,1,1);	//Diffuse light color
	//id  *= material.rgb;
	
	
	float3 diffuse_light = kd * id * amount_diffuse_light;
	
	
	// Specular lighting section
	float ks = 1.0;						//Specular reflection coefficient
	float3 is = float3(1.0, 1.0, 1.0);	//Specular light color
	
	//DIRECTION TO CAMERA
    float3 direction_to_camera = normalize(input.world_pos.xyz - m_camera_position.xyz);
	
    float3 reflected_light = reflect(light_dir.xyz, input.normal);
	
	float shininess = 100.0;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, direction_to_camera)), shininess);
	
	float3 specular_light = ks * amount_specular_light * is;
	
	
	float3 final_light = ambient_light + diffuse_light + specular_light;
	
	return float4(final_light, 1.0);
}