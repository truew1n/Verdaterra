#ifndef EN_TEXTURE_H
#define EN_TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"

#include "RenderObject.h"
#include "Handle.h"
#include "Bind.h"
#include "DeviceBuffer.h"


enum class ETextureType : uint32_t {
	Texture1D = GL_TEXTURE_1D,
	Texture2D = GL_TEXTURE_2D,
	Texture3D = GL_TEXTURE_3D,
	Texture1DArray = GL_TEXTURE_1D_ARRAY,
	Texture2DArray = GL_TEXTURE_2D_ARRAY,
	TextureRectangle = GL_TEXTURE_RECTANGLE,
	TextureCubeMap = GL_TEXTURE_CUBE_MAP,
	TextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
	TextureBuffer = GL_TEXTURE_BUFFER,
	Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
	Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};


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


enum class ETextureParameterValue : uint32_t {
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


enum class ETextureUsage : uint8_t {
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
	MayaSpecularRoughness = 25,
	None = 0
};


class CTexture : public CRenderObject, public CHandle, public IBind {
private:
	inline static uint32_t MMaxUnit = 0;

	ETextureUsage MUsage;
	uint32_t MUnit;

	int32_t MChannels;
private:
	uint32_t GetNextUnit();
public:
	virtual void Create() override;
	virtual void Create(ETextureType Type);
	virtual void Create(const char *Filepath);
	virtual void Create(const char *Filepath, ETextureType Type);
	virtual void Bind() override;

	void SetTextureParameter(ETextureParameter Type, ETextureParameterValue Value);
	
	ETextureUsage GetUsage() const;
	void SetUsage(ETextureUsage Usage);
	uint32_t GetUnit() const;
	uint32_t GetChannels() const;

	virtual void Unbind() override;
	virtual void Destroy() override;
};

#endif