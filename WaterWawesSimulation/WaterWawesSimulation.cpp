#include <Classes/Application.h>

int main()
{
	const std::unique_ptr<Application> app = std::make_unique<Application>();

	return app->Exec();
}
