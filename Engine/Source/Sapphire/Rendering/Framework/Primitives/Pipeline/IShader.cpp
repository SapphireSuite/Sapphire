#include <Rendering/Framework/Primitives/Pipeline/IShader.hpp>

namespace Sa
{
	ShaderType IShader::GetShaderType() const
	{
		return mShaderType;
	}
}