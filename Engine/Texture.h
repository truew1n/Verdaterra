#ifndef PE_TEXTURE_H
#define PE_TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"
#include "RenderObject.h"

enum class ETextureParameter : uint32_t {
	/*
		Possible values (ETextureParameterValue):
		- Nearest
		- Linear
		- NearestMipMapNearest
		- LinearMipMapNearest
		- NearestMipMapLinear
		- LinearMipMapLinear
	*/
	MinFilter = GL_TEXTURE_MIN_FILTER,
	/*
		Possible values (ETextureParameterValue):
		- Nearest
		- Linear
	*/
	MagFilter = GL_TEXTURE_MAG_FILTER,
	/*
		Possible values (ETextureParameterValue):
		- ClampToEdge
		- ClampToBorder
		- MirroredRepeat
		- Repeat
		- MirrorClampToEdge
	*/
	WrapU = GL_TEXTURE_WRAP_S,
	/*
		Possible values (ETextureParameterValue):
		- ClampToEdge
		- ClampToBorder
		- MirroredRepeat
		- Repeat
		- MirrorClampToEdge
	*/
	WrapV = GL_TEXTURE_WRAP_T
};

enum class ETextureParameterValue : int32_t {
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	NearestMipMapNearest = GL_NEAREST_MIPMAP_NEAREST,
	LinearMipMapNearest = GL_LINEAR_MIPMAP_NEAREST,
	NearestMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR,

	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER,
	MirroredRepeat = GL_MIRRORED_REPEAT,
	Repeat = GL_REPEAT,
	MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
};

enum class ETextureType : int8_t {
	None = 0,
	Diffuse = 1,
	Specular = 2,
	Ambient = 3,
	Emissive = 4,
	Height = 5,
	Normals = 6,
	Shininess = 7,
	Opacity = 8,
	Displacement = 9,
	Lightmap = 10,
	Reflection = 11,
	BaseColor = 12,
	NormalCamera = 13,
	EmissionColor = 14,
	Metalness = 15,
	DiffuseRoughness = 16,
	AmbientOcclusion = 17,
	Unknown = 18,
	Sheen = 19,
	ClearCoat = 20,
	Transmission = 21,
	MayaBase = 22,
	MayaSpecular = 23,
	MayaSpecularColor = 24,
	MayaSpecularRoughness = 25
};

class CTexture : public CRenderObject {
private:
	static uint32_t MMaxUnit;

	ETextureType MType;
	uint32_t MUnit;

	int32_t MChannels;
public:
	CTexture() : MType(ETextureType::None), MUnit(0), MChannels(0) {};
	CTexture(const char *ITexturePath);

	void SetTextureParameter(ETextureParameter Type, ETextureParameterValue Value);
	virtual void Bind() override;
	virtual void Unbind() override;
	~CTexture();

	ETextureType GetType() const { return MType; }
	void SetType(ETextureType Type) { MType = Type; }

	uint32_t GetUnit() const { return MUnit; }

	uint32_t GetChannels() const { return MChannels; }
};

#endif