#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex.h>
#include <string.h>
#include <signal.h>

#include "y.tab.h"
extern void getNext(enum yytokentype *, char *, int, int *);
#define TEXTSIZE 250

enum state
{
	START,
	COMMANDNAME,
	COMMANDLINEARG,
	INPUT,
	OUTPUT,
	STDERR,
	OUTPUTAPPEND,
	STDERRAPPEND,
	BACKGROUND,
	FILENAME,
	EXECUTE,
	ERROR
};

// Command Data Structure
// Describes a simple command and arguments
typedef struct SimpleCommand
{
	// Available space for arguments currently preallocated
	int _numberOfAvailableArguments;
	// Number of arguments
	int _numberOfArguments;
	// Array of arguments
	char *_arguments[];
} SimpleCommand;

// or global *_currentSimpleCommand instead of parameter
void insertArgument(SimpleCommand *command, char *argument);

// Describes a complete command with the multiple pipes if any
// and input/output redirection if any.
typedef struct Command
{
	int _numberOfAvailableSimpleCommands;
	int _numberOfSimpleCommands;
	SimpleCommand **_simpleCommands;
	char *_outFile;
	char *_inputFile;
	char *_errFile;
	int _background;
	int append;
} Command;

// this uses global static to keep track of current commands perhaps not the best way, works for objects not so well in C
void print();
void execute(Command *command);
void clear();
void insertSimpleCommand(SimpleCommand *simpleCommand, Command *command);
static Command _currentCommand;
static SimpleCommand _currentSimpleCommand;

/*PROGRAMNAME
Your shell must handle

·       command arguments
·       < input redirection
·       > stdout open for overwrite
·       >> stdout open for append
·       >& stderr overwrite
·       >>& stderr append
·       | pipes zero or more
*/

int table[8][10] = {
	// > = output redirect >> = output redirect append mode >>& stderr append mode
	//"NoToken", "GREAT", "NEWLINE", "WORD", "GREATGREAT", "PIPE", "LESS", "GREATAPERSAND", "GREATGREATAPERSAND", "AMPERSAND"
	{ERROR, ERROR, START, COMMANDNAME, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR},				//start state
	{ERROR, OUTPUT, EXECUTE, COMMANDLINEARG, OUTPUTAPPEND, START, INPUT, STDERR, STDERRAPPEND, BACKGROUND}, //command name state
	{ERROR, OUTPUT, EXECUTE, COMMANDLINEARG, OUTPUTAPPEND, START, INPUT, STDERR, STDERRAPPEND, BACKGROUND}, //command line argument
	{ERROR, ERROR, EXECUTE, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR},					//background
	{ERROR, ERROR, ERROR, FILENAME, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR},					//output append
	{ERROR, ERROR, ERROR, FILENAME, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR},					//stderr append 
	{ERROR, ERROR, ERROR, FILENAME, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR},					//input redirect
	{ERROR, ERROR, ERROR, FILENAME, ERROR, ERROR, ERROR, ERROR, ERROR, ERROR}					//output redirect
};

void printToken(enum yytokentype token)
{
	//0  	 	//1		//2			//3		//4			//5		//6			//7			//8 							//9
	static char *tokens[] = {"NoToken", "GREAT", "NEWLINE", "WORD", "GREATGREAT", "PIPE", "LESS", "GREATAPERSAND", "GREATGREATAPERSAND", "AMPERSAND"};
	printf("Token %s ", tokens[token - NOTOKEN]);
}

struct SimpleCommand *simpleCommand()
{
	//initialize the global currentsimplecommand struct
	SimpleCommand *_currentSimpleCommand = malloc(sizeof(*_currentSimpleCommand) + sizeof(char[50]));
	_currentSimpleCommand->_numberOfArguments = 0;
	_currentSimpleCommand->_numberOfAvailableArguments = 1000;
	_currentSimpleCommand->_arguments[_currentSimpleCommand->_numberOfArguments] = malloc(_currentSimpleCommand->_numberOfAvailableArguments * sizeof(char *));
	return _currentSimpleCommand;
}

struct Command *command()
{
	//initial the command struct
	Command *_currentCommand = (Command *)malloc(2 * sizeof(Command));
	_currentCommand->_numberOfAvailableSimpleCommands = 1;
	_currentCommand->_numberOfSimpleCommands = 0;
	_currentCommand->_background = 0;
	_currentCommand->append = 0;
	return _currentCommand;
}

int main(int argc, char *argv[])
{

	char text[TEXTSIZE];

	enum yytokentype token = NOTOKEN;

	enum state currentState = START; //starting state

	int textlength;

	printf("Enter the command : \n");

	char commandline[200][200];

	// initial the two struct
	SimpleCommand *_currentSimpleCommand = simpleCommand();
	Command *_currentCommand = command();


	while (token != NEWLINE)
	{
		getNext(&token, text, TEXTSIZE, &textlength);
		// printf("token is: %d\n", token);
		// printf("token is: %s\n", text);
		//code for lexical analyzer 260 -2 or something
		/*
		NOTOKEN = 258,
		GREAT = 259,
		NEWLINE = 260,
		WORD = 261,
		GREATGREAT = 262,
		PIPE = 263,
		LESS = 264,
		GREATAMPERSAND = 265,
		GREATGREATAMPERSAND = 266,
		AMPERSAND = 267
		*/

		currentState = table[currentState][token - NOTOKEN];
		int fileflag;
		int append;
		switch (currentState)
		{
		case START: 
			
			break;
		case COMMANDNAME:
			printf("program name\n");
			// insert the programname to the struct
			insertArgument(_currentSimpleCommand, text);
			insertSimpleCommand(_currentSimpleCommand, _currentCommand);

			break;
		//this case is for building the command line
		case COMMANDLINEARG:
			//insert the commandlinearg to the struct
			insertArgument(_currentSimpleCommand, text);
			insertSimpleCommand(_currentSimpleCommand, _currentCommand);
			
			break;
		case INPUT:
			//insert the inputredirect to the struct
			printf("input\n");
			fileflag = 0;
			break;
		case OUTPUT:
			//insert the outputredirect to the struct
			printf("output\n");

			fileflag = 1;
			append = 0;
			printf("in the > \n");
			
			break;
		case OUTPUTAPPEND:
			// printf("append: %d \n" , append);
			fileflag = 1;
			append = 1;
			printf("in the >> \n");
			// printf("token %s \n", text);
			// printf("%d \n" ,fileflag);
			break;
		case STDERR:
			//insert the stderr to the struct
			fileflag = 2;
			break;
		case STDERRAPPEND:
			fileflag = 2;
			append=1;
			break;
		case FILENAME:
			//insert the fillname to the struct
			printf("filename\n");
			if (fileflag == 0)
			{
				_currentCommand->_inputFile = text;
				_currentCommand->append = append;
			}
			else if (fileflag == 1)
			{
				_currentCommand->_outFile = text;
				_currentCommand->append = append;
			}
			else if (fileflag == 2)
			{
				_currentCommand->_errFile = text;
				_currentCommand->append = append;
			}

			// printf("%d \n", fileflag);
			// printf("in %s \n", _currentCommand->_inputFile);
			// printf("out %s \n", _currentCommand->_outFile);
			// printf("err %s \n", _currentCommand->_errFile);
			// printf("append: %d \n" , _currentCommand->append);
			execute(_currentCommand);
			printf("END FILENAME\n");
			break;
		case EXECUTE:
			//
			execute(_currentCommand);

			printf("END EXECUTE \n");
			break;
		case BACKGROUND:
			_currentCommand->_background = 1;
			break;
		case ERROR:
			printf("command not found\n");
			break;
		default:
			break;
		}
		// printf("CURstate token %d\n", token - NOTOKEN);

		// printToken(token);
		// printf("token %d, text >%s< \n",  token, text);
		clear();
	}
	return 0;
}

//insertArgument to simpleCommand struct
void insertArgument(SimpleCommand *command, char *argument)
{
	//give the memory address
	command->_arguments[command->_numberOfArguments] = (char *)realloc(command->_arguments[command->_numberOfArguments], sizeof(char *) * strlen(argument));
	strcpy(command->_arguments[command->_numberOfArguments], argument);
	command->_arguments[command->_numberOfArguments + 1] = NULL;
	command->_numberOfArguments++;
}

//insert Simplecommand to command struct
void insertSimpleCommand(SimpleCommand *simpleCommand, Command *command)
{
	command->_simpleCommands = realloc(command->_simpleCommands, sizeof(simpleCommand) * command->_numberOfSimpleCommands);
	SimpleCommand *tempCommand = simpleCommand;
	command->_simpleCommands[command->_numberOfSimpleCommands] = tempCommand;
	command->_numberOfSimpleCommands++;
}

void clear()
{
	for (int i = 0; i < _currentCommand._numberOfSimpleCommands; i++)
	{
		for (int j = 0; j < _currentCommand._simpleCommands[i]->_numberOfArguments; j++)
		{
			free(_currentCommand._simpleCommands[i]->_arguments[j]);
		}
		free(_currentCommand._simpleCommands[i]->_arguments);
		free(_currentCommand._simpleCommands);
	}
	if (_currentCommand._outFile)
	{
		free(_currentCommand._outFile);
	}
	if (_currentCommand._inputFile)
	{
		free(_currentCommand._inputFile);
	}
	if (_currentCommand._errFile)
	{
		free(_currentCommand._errFile);
	}
	_currentCommand._numberOfSimpleCommands = 0;
	_currentCommand._background = 0;
	_currentCommand.append = 0;
}

void print()
{
	if (isatty(0))
	{
		fflush(stdout);
	}
}

void execute(Command *command)
{

	//cd
	if (!strcmp(command->_simpleCommands[0]->_arguments[0], "cd"))
	{
		int ret;
		if (command->_simpleCommands[0]->_numberOfArguments == 1)
			ret = chdir(getenv("HOME"));
		else
			ret = chdir(command->_simpleCommands[0]->_arguments[1]);

		if (ret < 0)
			perror("cd");
		clear();
		print();
		return;
	}

	if (!strcmp(command->_simpleCommands[0]->_arguments[0], "clear"))
	{
		system("clear");
		clear();
		print();
		return;
	}

	int defaultin = dup(0);
	int defaultout = dup(1);
	int defaulterr = dup(2);

	int fdout, fdin, fderr;

	if (command->_inputFile != NULL)
	{
		fdin = open(command->_inputFile, O_RDONLY);
		if (fdin < 0)
		{
			perror("inputFile open error");
			exit(1);
		}
	}
	else
	{
		fdin = dup(defaultin);
	}
	if (command->_errFile != NULL)
	{
		if (command->append == 0)
			fderr = open(command->_errFile, O_CREAT | O_WRONLY | O_TRUNC);
		else
			fderr = open(command->_errFile, O_CREAT | O_WRONLY | O_APPEND);
		if (fderr < 0)
		{
			perror("errFile open error");
			exit(1);
		}
	}
	else
	{
		// use default error
		fderr = dup(defaulterr);
	}

	int ret;
	for (int i = 0; i < command->_numberOfSimpleCommands; i++)
	{
		// redirect input
		dup2(fdin, 0);
		dup2(fderr, 2);
		close(fdin);

		// setup output
		if (i == command->_numberOfSimpleCommands - 1) // Last simple command
		{
			if (command->_outFile != NULL)
			{
				if (command->append == 0){
					fdout = open(command->_outFile, O_CREAT | O_WRONLY | O_TRUNC, 0664);
				}
				else{
					fdout = open(command->_outFile, O_CREAT | O_WRONLY | O_APPEND, 0664);
				}
				if (fdout < 0)
				{
					perror("outFile open error");
					exit(1);
				}
			}
			else if (command->_errFile != NULL)
			{
				if (command->append == 0) 
					fdout = open(command->_errFile, O_CREAT | O_WRONLY | O_TRUNC, 0664);
				else
					fdout = open(command->_errFile, O_CREAT | O_WRONLY | O_APPEND, 0664);
				if (fdout < 0)
				{
					perror("outFile open error");
					exit(1);
				}
			}
			else
			{
				// use defauly output
				fdout = dup(defaultout);
			}
		}
		else
		{
			//Not last
			//create pipe
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		// Redirect output
		dup2(fdout, 1);
		close(fdout);


		ret = fork();
		if (ret == 0)
		{
			//child
			execvp(command->_simpleCommands[i]->_arguments[0], command->_simpleCommands[i]->_arguments);
			perror("execvp");
			_exit(1);
		}
		else if (ret < 0)
		{
			perror("fork");
			return;
		}
	}


	// Restore input, output, and error
	dup2(defaultin, 0);
	dup2(defaultout, 1);
	dup2(defaulterr, 2);

	// close
	close(defaultin);
	close(defaultout);
	close(defaulterr);

	if (!command->_background)
	{
		// wait for last process
		waitpid(ret, NULL, 0);
	}

	// Clear to prepare for next command
	clear();

	print();
	
} // execute