#include <easycc/tools/Convertible.h>

#include <getopt.h>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <fstream>
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)
namespace ecc{
    void Convertible::initParams(int argc, char *argv[]) {

        struct option longOptions[] = {
                {"verbose", no_argument, 0, 'v'},
                {"template", required_argument, 0, 't'},
                {"output", required_argument, 0, 'o'},
                {"input", required_argument, 0, 'i'},
                {"help", no_argument, 0, 'h'},
                {0, 0, 0, 0}
        };

        int optionIndex = 0;
        int c;
        while ((c = getopt_long(argc, argv, "hvo:t:i:", longOptions, &optionIndex)) != -1) {
            switch (c) {
                case 'v':
                    logging::add_console_log(
                            std::cout,
                            boost::log::keywords::format = "[%TimeStamp%]: %Message%"
                    );
                    logging::core::get()->set_logging_enabled(true);
                    break;
                case 'o':
                    m_outputFile = optarg;
                    break;
                case 't':
                    m_templateFile = optarg;
                    break;
                case 'i':
                    m_inputFile = optarg;
                    break;
                case 'h':
                default:
                    // Print by default
                    break;
            }
        }
    }

    void Convertible::printUsage(std::string header, std::string footer) {
        std::cout
        << header << std::endl
        << "\t-t, --template\t\tTemplate file" << std::endl
        << "\t-o, --output\t\tOutput file" << std::endl
        << "\t-i, --input\t\tInput file" << std::endl
        << "\t-v, --verbose\t\tVerbose mode" << std::endl
        << "\t-h, --help\t\tDisplay this help message" << std::endl << std::endl
        << footer << std::endl;
    }

    int Convertible::init(int argc, char **argv) {
        initParams(argc, argv);

        // Init logs
        logging::add_common_attributes();
        logging::core::get()->set_logging_enabled(false);

        // Check if all the required arguments are passed
        if(m_inputFile.empty() || m_outputFile.empty() || m_templateFile.empty()) {
            return Convertible::RETURN_ERROR;
        }
        return Convertible::RETURN_SUCCESS;
    }

    int Convertible::run() {

        // Prepare output file
        std::ofstream oFile(m_outputFile);

        // Check if can open file
        if (!oFile.is_open()) {
            std::cerr << "Cannot open file: " << m_outputFile << std::endl;
            return RETURN_ERROR;
        } else {

            // Load template into string
            std::ifstream inputFile(m_templateFile);

            if (!inputFile.is_open()) {
                std::cerr << "Cannot open file: " << m_templateFile << std::endl;
                return RETURN_ERROR;
            } else {

                // Store template into string
                std::stringstream buffer;
                buffer << inputFile.rdbuf();
                m_newContent = buffer.str();

                // Close file because its content is copied
                inputFile.close();

                // Convert file
                convert(m_inputFile);

                // Write new content into output file
                oFile << m_newContent;
            }

            // Close output file
            oFile.close();
        }
        return RETURN_SUCCESS;
    }
}
