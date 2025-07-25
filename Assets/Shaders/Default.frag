#version 460 core

out vec4 EFragmentColor;

in vec3 EPosition;
in vec3 ENormal;
in vec2 EUV;
in vec4 ELightFragmentPosition;

uniform bool UBTexture;
uniform vec4 UColorDiffuse = vec4(1.0);
uniform sampler2D UTextureDiffuse;
uniform sampler2D UShadowMap;

uniform vec4 ULightColor = vec4(1.0);
uniform vec3 ULightPosition;
uniform vec3 UCameraPosition;

struct LightingComponents {
    float Diffuse;
    float Specular;
};


vec4 SampleColor()
{
    if(UBTexture) {
        return texture(UTextureDiffuse, EUV);
    }
    return UColorDiffuse;
}

float InverseQuadratic(float X, float A, float B, float C)
{
    return 1.0f / (A * X * X + B * X + C);
}

LightingComponents ComputeLightingComponents(vec3 Normal, vec3 LightDirection, vec3 ViewDirection)
{
    LightingComponents Components;
    Components.Diffuse = max(dot(Normal, LightDirection), 0.0f);
    Components.Specular = 0.0f;
    if(Components.Diffuse > 0.0f) {
        vec3 HalfwayVector = normalize(ViewDirection + LightDirection);
        float SpecularAmount = pow(max(dot(Normal, HalfwayVector), 0.0f), 16);
        Components.Specular = SpecularAmount * 0.50f;
    }
    return Components;
}

float ComputeShadow(vec3 LightCoordinates, float Bias)
{
    if(LightCoordinates.z > 1.0f) {
        return 0.0f;
    }
    LightCoordinates = (LightCoordinates + 1.0f) / 2.0f;
    float CurrentDepth = LightCoordinates.z;

    int SampleRadius = 2;
    vec2 PixelSize = 1.0f / textureSize(UShadowMap, 0);
    float Shadow = 0.0f;
    for(int y = -SampleRadius; y <= SampleRadius; ++y) {
        for(int x = -SampleRadius; x <= SampleRadius; ++x) {
            float ClosestDepth = texture(UShadowMap, LightCoordinates.xy + vec2(x, y) * PixelSize).r;
            if(CurrentDepth > (ClosestDepth + Bias)) {
                Shadow += 1.0f;
            }
        }
    }
    Shadow /= pow(SampleRadius * 2 + 1, 2);
    return Shadow;
}

vec4 PointLight()
{
    vec3 LightVector = ULightPosition - EPosition;
    float Distance = length(LightVector);
    float Intensity = InverseQuadratic(Distance, 0.000005, 0.01, 1.0);

    vec3 Normal = normalize(ENormal);
    vec3 ViewDirection = normalize(UCameraPosition - EPosition);
    vec3 LightDirection = normalize(LightVector);
    LightingComponents Components = ComputeLightingComponents(Normal, LightDirection, ViewDirection);

    vec3 LightCoordinates = ELightFragmentPosition.xyz / ELightFragmentPosition.w;
    float Bias = 0.0005f;
    float Shadow = ComputeShadow(LightCoordinates, Bias);
    float ShadowFactor = 1.0 - Shadow;

    float Ambient = 0.0f;
    float TotalLighting = Ambient + Components.Diffuse * Intensity * ShadowFactor + Components.Specular * Intensity * ShadowFactor;
    vec4 Color = SampleColor();
    return Color * TotalLighting * ULightColor;
}

vec4 DirectionalLight()
{
    vec3 Normal = normalize(ENormal);
    vec3 ViewDirection = normalize(UCameraPosition - EPosition);
    vec3 LightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
    LightingComponents Components = ComputeLightingComponents(Normal, LightDirection, ViewDirection);

    float Ambient = 0.0f;
    float TotalLighting = Ambient + Components.Diffuse + Components.Specular;
    vec4 Color = SampleColor();
    return Color * TotalLighting * ULightColor;
}

vec4 SpotLight()
{
    vec3 LightDirection = normalize(ULightPosition - EPosition);
    float Angle = dot(vec3(0.0f, -1.0f, 0.0f), -LightDirection);
    float Intensity = clamp((Angle - 0.80f) / (0.95f - 0.80f), 0.0f, 1.0f);

    vec3 Normal = normalize(ENormal);
    vec3 ViewDirection = normalize(UCameraPosition - EPosition);
    LightingComponents Components = ComputeLightingComponents(Normal, LightDirection, ViewDirection);

    float Ambient = 0.0f;
    float TotalLighting = Ambient + Components.Diffuse * Intensity + Components.Specular * Intensity;
    vec4 Color = SampleColor();
    return Color * TotalLighting * ULightColor;
}

void main()
{
    EFragmentColor = PointLight();
}