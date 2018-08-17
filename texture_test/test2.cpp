#include <fstream>
#include <string>

#include <kanzi/kanzi.hpp>
#include <kanzi/core.ui/application/application.hpp>
#include <kanzi/core/cpp/math.hpp>

#include "test2.hpp"

namespace kanzi {

	void Test2::onConfigure(ApplicationProperties& configuration)
	{
		configuration.binaryName = "test2.kzb.cfg";
	}

	Image2DSharedPtr Test2::Create_image(Domain * domain, string path) {
		TextureSharedPtr texture = domain->getResourceManager()->acquireResource<Texture>(string("file://./") + path);
		return Image2D::create(domain, "image_node", texture);
	}

	void Test2::onProjectLoaded()
	{
		kanzi::Application::initializeGL();
		Domain * domain = getDomain();

		GridLayout2DSharedPtr grid = GridLayout2D::create(domain, "grid");
		grid->addAutomaticColumn();
		grid->addAutomaticColumn();
		grid->addAutomaticRow();
		grid->addAutomaticRow();
		ScreenSharedPtr screen = getScreen();
		ifstream mfile ("path.txt");
		string path;
		if (mfile.is_open()) {
			while (std::getline(mfile, path)) {
				if (path.empty()) {
					continue;
				}
				kzLogInfo(KZ_LOG_CATEGORY_GENERIC, ("{}\n", path));
				Image2DSharedPtr imageptr = Create_image(domain, path);
				grid->addChild(imageptr);
				
			}
			mfile.close();
		}

		TextBlock2DSharedPtr text = TextBlock2D::create(domain,"text_node","Test text");
		screen->addChild(text);
		screen->addChild(grid);

	}

	void Test2::registerMetadataOverride(ObjectFactory& /*factory*/)
	{
		KanziComponentsModule::registerModule(getDomain());
		V8Module::registerModule(getDomain());
	}
}

kanzi::Application* createApplication()
{
	return new kanzi::Test2;
}
