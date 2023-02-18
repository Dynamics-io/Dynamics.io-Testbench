#include "Logger.h"

using namespace Dynamics_IO_Testbench;

Dynamics_IO_Testbench::Logger Logger::m_logger;

void Dynamics_IO_Testbench::Logger::Log(Logger::Level level, std::string source, std::string message)
{
	LogEntry entry;
	entry.log_level = level;
	entry.source = source;
	entry.message = message;

	m_logger.m_entries.push_back(entry);
}

void Dynamics_IO_Testbench::Logger::LogError(std::string source, std::string message)
{
	Log(Level::Error, source, message);
}

void Dynamics_IO_Testbench::Logger::LogWarning(std::string source, std::string message)
{
	Log(Level::Warning, source, message);
}

void Dynamics_IO_Testbench::Logger::LogInfo(std::string source, std::string message)
{
	Log(Level::Info, source, message);
}

void Dynamics_IO_Testbench::Logger::LogDebug(std::string source, std::string message)
{
	Log(Level::Debug, source, message);
}

std::vector<Logger::LogEntry> Dynamics_IO_Testbench::Logger::GetLogEntries()
{
	// Declaring new vector
	std::vector<Logger::LogEntry> res;

	// Copying vector by copy function
	std::copy(m_logger.m_entries.begin(), m_logger.m_entries.end(), std::back_inserter(res));

	m_logger.m_entries.clear();

	return res;
}


