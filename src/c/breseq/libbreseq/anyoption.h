#ifndef _ANYOPTION_H
#define _ANYOPTION_H

#include "common.h"

#define COMMON_OPT 	1
#define COMMAND_OPT 	2
#define FILE_OPT 	3
#define COMMON_FLAG 	4
#define COMMAND_FLAG 	5
#define FILE_FLAG 	6

#define COMMAND_OPTION_TYPE  	1
#define COMMAND_FLAG_TYPE 	2
#define FILE_OPTION_TYPE  	3
#define FILE_FLAG_TYPE 		4
#define UNKNOWN_TYPE 		5

#define MAX_LONG_PREFIX_LENGTH 	2

#define DEFAULT_MAXHELP         10

#define TRUE_FLAG "true"
#define ADVANCED_OPTION true

#define TAKES_NO_ARGUMENT	((void*)NULL)
#define USAGE_LEFT_COLUMN_WIDTH	35

using namespace std;

namespace breseq {

	class AnyOption
	{

	public: /* the public interface */
		AnyOption();
		AnyOption(const string& usage_prefix);
		~AnyOption();

		/*
			 * following set methods specifies the
		 * special characters and delimiters
		 * if not set traditional defaults will be used
			 */

		void setCommandPrefixChar( char _prefix );   /* '-' in "-w" */
		void setCommandLongPrefix( char *_prefix );  /* '--' in "--width" */
		void setFileCommentChar( char _comment );    /* '#' in shellscripts */
		void setFileDelimiterChar( char _delimiter );/* ':' in "width : 100" */

		/*
		 * provide the input for the options
			 * like argv[] for commndline and the
			 * option file name  to use;
		 */

		void useCommandArgs( int _argc, char** _argv );
		void useFilename( const char *_filename );

		/*
			 * turn off the POSIX style options
			 * this means anything starting with a '-' or "--"
			 * will be considered a valid option
			 * which alo means you cannot add a bunch of
			 * POIX options chars together like "-lr"  for "-l -r"
			 *
			 */

		void noPOSIX();

		/*
			 * prints warning verbose if you set anything wrong
			 */
		void setVerbose();


		/*
			 * there are two types of options
			 *
			 * Option - has an associated value ( -w 100 )
			 * Flag  - no value, just a boolean flag  ( -nogui )
			 *
		 * the options can be either a string ( GNU style )
			 * or a character ( traditional POSIX style )
			 * or both ( --width, -w )
			 *
			 * the options can be common to the commandline and
			 * the optionfile, or can belong only to either of
			 * commandline and optionfile
			 *
			 * following set methods, handle all the aboove
		 * cases of options.
			 */

		/* options comman to command line and option file */
		void setOption( const char *opt_string );
		void setOption( char  opt_char );
		void setOption( const char *opt_string , char opt_char );
		void setFlag( const char *opt_string );
		void setFlag( char  opt_char );
		void setFlag( const char *opt_string , char opt_char );

		/* options read from commandline only */
		void setCommandOption( const char *opt_string );
		void setCommandOption( char  opt_char );
		void setCommandOption( const char *opt_string , char opt_char );
		void setCommandFlag( const char *opt_string );
		void setCommandFlag( char  opt_char );
		void setCommandFlag( const char *opt_string , char opt_char );

		/* options read from an option file only  */
		void setFileOption( const char *opt_string );
		void setFileOption( char  opt_char );
		void setFileOption( const char *opt_string , char opt_char );
		void setFileFlag( const char *opt_string );
		void setFileFlag( char  opt_char );
		void setFileFlag( const char *opt_string , char opt_char );

		/*
			 * process the options, registerd using
			 * useCommandArgs() and useFileName();
			 */
		void processOptions();
		void processCommandArgs();
		void processCommandArgs( int max_args );
		bool processFile();

		/*
			 * process the specified options
			 */
		void processCommandArgs( int _argc, char** _argv );
		void processCommandArgs( int _argc, char** _argv, int max_args );
		bool processFile( const char *_filename );

		/*
			 * get the value of the options
		 * will return NULL if no value is set
			 */
		string* getValue( const string option );
		bool  getFlag( const string option );
		string* getValue( char _optchar );
		bool  getFlag( char _optchar );
    
    /*
     * is this option used in the command line at all
     */
		bool  isOptUsed( const string option );
    bool  isOptUsed( char _optchar );

		/*
		 * Print Usage
		 */
		void printUsage();
		void printAutoUsage();
		void addUsage( string line , bool advanced=false);
    void addUsageSameLine( string line , bool advanced=false);
		void printAdvancedUsage();
			/* print auto usage printing for unknown options or flag */
		void autoUsagePrint(bool flag);

		/*
			 * get the argument count and arguments sans the options
			 */
		int   getArgc();
		char* getArgv( int index );
    vector<string> getRemainingArgs() {
      vector<string> return_vector;
      for (int32_t i = 0; i < getArgc(); i++)
      {
        string read_file_name = getArgv(i);
        return_vector.push_back(read_file_name);
      }
      return return_vector;
    }
    
		bool  hasOptions();

	private:
		string word_wrap(string sentence, int width);
		template<class T> void addOptionOrFlag(const string& option_name, const string& option_description, const T& option_default_value, bool has_argument, bool has_default_value, bool advanced)
		{
      // split the name first 
      vector<string> option_name_split = split(option_name, ",");
      string default_value_string = to_string(option_default_value);

      // assign default value to both long and short option name variants
      if (has_default_value) 
      {
        for (vector<string>::iterator it=option_name_split.begin(); it<option_name_split.end(); it++) 
        {
          default_values[*it] = default_value_string;
        }
      }      
      
      string usage;
			if (option_name_split.size() > 1)
			{
				if (has_argument)
					this->setOption(option_name_split[0].c_str(), option_name_split[1][0]);
				else
					this->setFlag(option_name_split[0].c_str(), option_name_split[1][0]);
				usage = "  -" + option_name_split[1] + ",--" + option_name_split[0];
			}
			else
			{
				if (has_argument)
					this->setOption(option_name_split[0].c_str());
				else
					this->setFlag(option_name_split[0].c_str());

        usage = "  --" + option_name_split[0];
			}

			if (has_argument) usage += " <arg>";

      // Pad with spaces to reach right side
			if (usage.size() < USAGE_LEFT_COLUMN_WIDTH)
				usage += string(abs(static_cast<int32_t>(USAGE_LEFT_COLUMN_WIDTH - usage.size())), ' ');
      // option and argument part bleeds over into right column, pad to make it take up a whole terminal line by itself.
      else if (usage.size()+1 >= USAGE_LEFT_COLUMN_WIDTH)
        usage += string(abs(static_cast<int32_t>(terminal_width - usage.size())), ' ')
          + '\n' + string(abs(static_cast<int32_t>(USAGE_LEFT_COLUMN_WIDTH)), ' ');
        
      // Add the default option information
      string default_value_description("");
      if (has_default_value)
			{
				if (default_value_string.size() > 0)
					default_value_description = " (DEFAULT=" + default_value_string + ")";
//				else
//					default_value_description = " (OPTIONAL)";
			}
      
			string wrapped_description = word_wrap(option_description + default_value_description, terminal_width - USAGE_LEFT_COLUMN_WIDTH);
			string search_string = "\n";
			string replace_string = search_string + string(USAGE_LEFT_COLUMN_WIDTH, ' ');
			string::size_type pos = 0;
			while ( (pos = wrapped_description.find(search_string, pos)) != string::npos ) {
				wrapped_description.replace(pos, search_string.size(), replace_string);
				pos += replace_string.size();
			}
			usage += wrapped_description;

			addUsage(usage, advanced);
		}

	public:
		AnyOption& operator()(const string& option_name, const string& option_description, bool advanced=false)
		{
			addOptionOrFlag(option_name, option_description, "", true, false, advanced);
			return *this;
		}
		AnyOption& operator()(const string& option_name, const string& option_description, void* pass_null_if_option_takes_no_argument, bool advanced=false)
		{
			assert(pass_null_if_option_takes_no_argument == NULL);
			addOptionOrFlag(option_name, option_description, "", false, false, advanced);
			return *this;
		}
    AnyOption& operator()(const string& option_name, const string& option_description, const char* option_default_value, bool advanced=false)
		{
			addOptionOrFlag(option_name, option_description, string(option_default_value), true, true, advanced);
			return *this;
		}
    
		template<class T> AnyOption& operator()(const string& option_name, const string& option_description, const T& option_default_value, bool advanced=false)
		{
			addOptionOrFlag(option_name, option_description, option_default_value, true, true, advanced);
			return *this;
		}

		string operator[](const string& option_name);
		bool count(const string& option_name);



	private: /* the hidden data structure */
		int argc;		/* commandline arg count  */
		char** argv;  		/* commndline args */
		const char* filename; 	/* the option file */
		char* appname; 	/* the application name from argv[0] */

		int *new_argv; 		/* arguments sans options (index to argv) */
		int new_argc;   	/* argument count sans the options */
		int max_legal_args; 	/* ignore extra arguments */


		/* option strings storage + indexing */
		vector<string> options; 	/* storage */		
    vector<char> optionchars; 	/*  storage */
		vector<int32_t> optiontype; 	/* type - common, command, file */
		vector<int32_t> optionindex;	/* index into value storage */
    
    set<string> assigned_options; /* keep track of previously assigned options */

		/* values */
		map <int, string> values; 		/* common value storage */
		int g_value_counter; 	/* globally updated value index LAME! */

		/* help and usage */
		vector<string> usage_lines; 	/* usage */
    vector<string> advanced_lines; 	/* advanced usage */

		bool command_set;	/* if argc/argv were provided */
		bool file_set;		/* if a filename was provided */
		bool posix_style; 	/* enables to turn off POSIX style options */
		bool verbose;		/* silent|verbose */
		bool print_usage;	/* usage verbose */
		bool print_help;	/* help verbose */

		char opt_prefix_char;		/*  '-' in "-w" */
		char long_opt_prefix[MAX_LONG_PREFIX_LENGTH+1]; /* '--' in "--width" */
		char file_delimiter_char;	/* ':' in width : 100 */
		char file_comment_char;		/*  '#' in "#this is a comment" */
		char equalsign;
		char comment;
		char delimiter;
		char endofline;
		char whitespace;
		char nullterminate;

		bool once;  //was static member

		bool hasoptions;
		bool autousage;



		int terminal_width;
		map<string, string> default_values;



	private: /* the hidden utils */
		void init();
		bool valueStoreOK();

		bool setValue( const string option , string value );
		bool setFlagOn( const string option );
		bool setValue( char optchar , string value);
		bool setFlagOn( char optchar );

		void addOption( const string option , char optchar, int type );

		bool findFlag( int index );
		void addUsageError( string line );
		bool CommandSet();
		bool FileSet();
		bool POSIX();

		char parsePOSIX( char* arg );
		int parseGNU( char *arg );
		bool matchChar( char c );
		int matchOpt( char *opt );

		/* dot file methods */
		char *readFile();
		char *readFile( const char* fname );
		bool consumeFile( char *buffer );
		void processLine( char *theline, int length );
		char *chomp( char *str );
		void valuePairs( char *type, string value );
		void justValue( char *type );

		void printVerbose( const char *msg );
		void printVerbose( char *msg );
		void printVerbose( char ch );
		void printVerbose( );

	}; // class AnyOption

} // namespace breseq
#endif /* ! _ANYOPTION_H */
