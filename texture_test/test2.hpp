#pragma once

#include <kanzi/kanzi.hpp>


namespace kanzi {

	class Test2 : public Application
	{
	public: 
		Image2DSharedPtr Create_image(Domain * domain, string path);
	protected:
		virtual void onConfigure(ApplicationProperties& configuration) KZ_OVERRIDE;
		virtual void onProjectLoaded() KZ_OVERRIDE;
		virtual void registerMetadataOverride(ObjectFactory& /*factory*/) KZ_OVERRIDE;
	};
}