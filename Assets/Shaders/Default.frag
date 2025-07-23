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

vec4 PointLight()
{
    vec3 LightVector = ULightPosition - EPosition;
    float Distance = length(LightVector);
    float A = 0.005;
    float B = 0.01;
    float C = 1.0f;
    float Intensity = InverseQuadratic(Distance, A, B, C);

    float Ambient = 0.0f;

    vec3 Normal = normalize(ENormal);
    vec3 LightDirection = normalize(ULightPosition - EPosition);
    float Diffuse = max(dot(Normal, LightDirection), 0.0f);

    float Specular = 0.0f;
    if(Diffuse != 0.0f) {
        float SpecularLight = 0.50f;
        vec3 ViewDirection = normalize(UCameraPosition - EPosition);
        vec3 ReflectionDirection = reflect(-LightDirection, Normal);
    
        vec3 HalfwayVector = normalize(ViewDirection + LightDirection);

        float SpecularAmount = pow(max(dot(Normal, HalfwayVector), 0.0f), 16);
        Specular = SpecularAmount * SpecularLight;
    }

    float Shadow = 0.0f;
    vec3 LightCoordinates = ELightFragmentPosition.xyz / ELightFragmentPosition.w;
    if(LightCoordinates.z <= 1.0f) {
        LightCoordinates = (LightCoordinates + 1.0f) / 2.0f;
        float CurrentDepth = LightCoordinates.z;
        float Bias = 0.0005f;// max(0.025f * (1.0f - dot(Normal, LightDirection)), 0.0005f);

        int SampleRadius = 2;
        vec2 PixelSize = 1.0f / textureSize(UShadowMap, 0);
        for(int y = -SampleRadius; y <= SampleRadius; ++y) {
            for(int x = -SampleRadius; x <= SampleRadius; ++x) {
                float ClosestDepth = texture(UShadowMap, LightCoordinates.xy + vec2(x, y) * PixelSize).r;
                if(CurrentDepth > (ClosestDepth + Bias)) {
                    Shadow += 1.0f;
                }
            }
        }

        Shadow /= pow(SampleRadius * 2 + 1, 2);
    }

    return (SampleColor() * ((Diffuse * Intensity * (1.0 - Shadow)) + Ambient + (Specular * Intensity * (1.0 - Shadow)))) * ULightColor;
}

vec4 DirectionalLight()
{
    float Ambient = 0.0f;

    vec3 Normal = normalize(ENormal);
    vec3 LightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
    float Diffuse = max(dot(Normal, LightDirection), 0.0f);

    float Specular = 0.0f;
    if(Diffuse != 0.0f) {
        float SpecularLight = 0.50f;
        vec3 ViewDirection = normalize(UCameraPosition - EPosition);
        vec3 ReflectionDirection = reflect(-LightDirection, Normal);
    
        vec3 HalfwayVector = normalize(ViewDirection + LightDirection);

        float SpecularAmount = pow(max(dot(Normal, HalfwayVector), 0.0f), 16);
        Specular = SpecularAmount * SpecularLight;
    }

    return (SampleColor() * (Diffuse + Ambient + Specular)) * ULightColor;
}

vec4 SpotLight()
{
    float OuterCone = 0.80f;
    float InnerCone = 0.95f;

    float Ambient = 0.0f;

    vec3 Normal = normalize(ENormal);
    vec3 LightDirection = normalize(ULightPosition - EPosition);
    float Diffuse = max(dot(Normal, LightDirection), 0.0f);

    float Specular = 0.0f;
    if(Diffuse != 0.0f) {
        float SpecularLight = 0.50f;
        vec3 ViewDirection = normalize(UCameraPosition - EPosition);
        vec3 ReflectionDirection = reflect(-LightDirection, Normal);
    
        vec3 HalfwayVector = normalize(ViewDirection + LightDirection);

        float SpecularAmount = pow(max(dot(Normal, HalfwayVector), 0.0f), 16);
        Specular = SpecularAmount * SpecularLight;
    }

    float Angle = dot(vec3(0.0f, -1.0f, 0.0f), -LightDirection);
    float Intensity = clamp((Angle - OuterCone) / (InnerCone - OuterCone), 0.0f, 1.0f);

    return (SampleColor() * ((Diffuse * Intensity) + Ambient + (Specular * Intensity))) * ULightColor;
}

void main()
{
    /*if(UBTexture) {
        EFragmentColor = vec4(vec3(texture(UTextureDiffuse, EUV).r), 1.0f);
    } else {
        EFragmentColor = UColorDiffuse;
    }*/

    EFragmentColor = PointLight();
}