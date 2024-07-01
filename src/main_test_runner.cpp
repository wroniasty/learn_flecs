#include <iostream>
#include <flecs.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <Windows.h>
#include <catch2/catch_session.hpp>

#include <format>
#include "modules/position.h"
#include "modules/scenegraph.h"


int main(int argc, char **argv) {

	Catch::Session session; // There must be exactly one instance

	int height = 0; // Some user variable you want to be able to set

	// Build a new parser on top of Catch2's
	using namespace Catch::Clara;

	auto cli
		= session.cli();     // Get Catch2's command line parser

	// Now pass the new composite back to Catch2 so it uses that
	session.cli(cli);
	//session.configData().showDurations = Catch::ShowDurations::Always;
	session.configData().verbosity = Catch::Verbosity::High;

	// Let Catch2 (using Clara) parse the command line
	int returnCode = session.applyCommandLine(argc, argv);
	if (returnCode != 0) // Indicates a command line error
		return returnCode;

	// if set on the command line then 'height' is now set at this point

	return session.run();
}