/*
 * main.c
 *
 *  Created on: Mar 13, 2018
 *      Author: nickyang
 */
#include "cli.h"
#include "stdio.h"
#include "string.h"
#include "sys/time.h"
#include "stdlib.h"

/*!@brief A simple example of handling un-used args. Just print them.
 *
 * @param argc  Argument Count
 * @param args  Pointer to Argument String
 * @return      Error Code
 */
int PrintArgs(int argc, char **args)
{
    if ((argc == 0) || (args == 0))
    {
        return 0;
    }

    //Example of a call back to handle un-used args.
    //Pass un-used args back;
    int i;
    printf("Un-used argc = [%d]\nUn-used args = ", argc);
    for (i = 0; i < argc; i++)
    {
        printf("%s ", args[i]);
    }
    printf("\n");
    return 0;
}

/*!@brief Handler for command "test".
 *        An example to get 1x interger value, 1x string value, 1x bool value.
 *
 * @param argc  Argument Count
 * @param args  Pointer to Argument String
 * @return      Error Code
 */
int Command_test(int argc, char *args[])
{
    //It's Recommended to build a temperory struct to store result.
    struct DataStruct
    {
        int IntData;
        char StringData[256];
        _Bool BoolData;
    } Tempdata;

    memset(&Tempdata, 0, sizeof(Tempdata));

    //Build the option list for main
    stCliOption MainOpt[] =
    {
    { OPT_COMMENT, 0, NULL, "Basic Options", NULL },
    { OPT_HELP, 'h', "help", "Show help hints", NULL },
    { OPT_INT, 'i', "int", "Get a Integer value", (void*) &Tempdata.IntData },
    { OPT_STRING, 's', "string", "Get a String value", (void*) Tempdata.StringData },
    { OPT_BOOL, 'b', "bool", "Get a Boolean value", (void*) &Tempdata.BoolData },
    { OPT_END, 0, NULL, NULL, NULL, PrintArgs } };

    //Run Arguments parse using MainOpt
    Cli_parseArgs(argc, args, MainOpt);

    //Print Result
    printf("\nResult: Int[%d] String[%s] Bool[%d]\n", Tempdata.IntData, Tempdata.StringData, Tempdata.BoolData);

    return 0;
}

/*!@brief Handler for command "time". Print Unix time stamp.
 */
int Command_time(int argc, char *args[])
{
    printf("Current Time:[%ld]", time(NULL));
    return 0;
}

/*!@brief Handler for command "quit". Quit process.
 */
int Command_quit(int argc, char *args[])
{
    exit(0);
}

int main(int argc, char *args[])
{
    stCliCommand MainCmd[] =
    {
    { "test", Command_test, "Run a argument parse example." },
    { "time", Command_time, "Get current time stamp" },
    { "quit", Command_quit, "Quit the process" },
    { NULL, NULL } };

    //Run initial commands
    Cli_excuteCommand(argc - 1, ++args, MainCmd);

    //Start a Mini-Terminal
    while (1)
    {
        //Get a command string
        char sbuf[256] =
        { 0 };
        printf(">");
        Cli_getCommand(sbuf);

        //Convert String to Args
        char *argbuf[16] =
        { 0 };
        int argcount = 0;
        Cli_convertStrToArgs(sbuf, &argcount, argbuf);

        //Run commands with Args
        Cli_excuteCommand(argcount, argbuf, MainCmd);
    }

    return 0;
}
