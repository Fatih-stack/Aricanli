#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <mutex>
#include <ctime>
#include <fstream>
namespace aricanli {
	namespace general {
		enum class Severity
		{
			none = 0, fatal, error, warn, info, debug, verbose
		};

		class Logger
		{
		private:
			Severity severity = Severity::none;
			std::mutex log_mutex;
			std::string file_path = "";
			std::ofstream file;

		protected:
			template <typename T>
			void log_argument(T t) {
				std::cout << t << " ";
			}
			template <typename T>
			void log_writefile(T t) {
				file << t << " ";
			}

		public:
			static void SetPriority(Severity new_severity)
			{
				get_instance().severity = new_severity;
			}

			template<typename... Args>
			static void Fatal(int line, const std::string& source_file, const std::string& message, Args... args)
			{
				get_instance().log(line, source_file, "[Fatal]\t", Severity::fatal, message, args...);
			}

			template<typename... Args>
			static void Error(int line, const std::string& source_file, const std::string& message, Args... args)
			{
				get_instance().log(line, source_file, "[Error]\t", Severity::error, message, args...);
			}

			template<typename... Args>
			static void Info(int line, const std::string& source_file, const std::string& message, Args... args)
			{
				get_instance().log(line, source_file, "[Info]\t", Severity::info, message, args...);
			}

			template<typename... Args>
			static void Warn(int line, const char* source_file, const char* message, Args... args)
			{
				get_instance().log(line, source_file, "[Warn]\t", Severity::warn, message, args...);
			}

			template<typename... Args>
			static void Debug(int line, const char* source_file, const char* message, Args... args)
			{
				get_instance().log(line, source_file, "[Debug]\t", Severity::debug, message, args...);
			}

			template<typename... Args>
			static void Verbose(int line, const char* source_file, const char* message, Args... args)
			{
				get_instance().log(line, source_file, "[Verbose]\t", Severity::verbose, message, args...);
			}

		private:
			Logger()
			{
				file_path = "C:\\Users\\hp\\source\\repos\\log.txt";
				enable_file_output(file_path);
			}

			Logger(const Logger&) = delete;
			Logger& operator= (const Logger&) = delete;

			~Logger()
			{
				if (file.is_open())
					file.close();
			}

			static Logger& get_instance()
			{
				static Logger logger;
				return logger;
			}

			template<typename... Args>
			void log(int line, const std::string& source, const std::string& msg_priorty_str,
				Severity msg_severity, const std::string& msg, Args... args)
			{
				if (severity <= msg_severity)
				{
					time_t current_time = time(0);
					tm* timestamp = localtime(&current_time);
					char buffer[80];
					strftime(buffer, 80, "%c", timestamp);
					std::string s(buffer);
					typename std::lock_guard lock(log_mutex);
					std::cout << s << '\t' << msg_priorty_str << " " << msg;
					int dummy[] = { 0, ((void)log_argument(std::forward<Args>(args)),0)... };
					std::cout << " on line " << line << " in " << source << std::endl;

					if (file.is_open())
					{
						file << s << '\t' << msg_priorty_str << " " << msg.c_str() << " ";
						int dummy[] = { 0, ((void)log_writefile(std::forward<Args>(args)),0)... };
						file << " on line " << line << " in " << source << "\n";
					}
					else
						std::cout << "Logger: Failed to open file " << file_path << std::endl;
				}
			}

			void enable_file_output()
			{
				file.close();
				file.open(file_path);
			}

			void enable_file_output(const std::string& new_file_path)
			{
				file.close();
				file_path = new_file_path;
				file.open(file_path);
			}
		};
	}
}
