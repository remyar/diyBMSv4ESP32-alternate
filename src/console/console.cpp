//================================================================================================//
//                                                                                                //
// PROJET       : DongleWifi GoodRace                                                             //
// MODULE       : Board                                                                           //
// DESCRIPTION  :                                                                                 //
// CREATION     : 27/01/2020                                                                      //
// HISTORIQUE   :                                                                                 //
//                                                                                                //
//================================================================================================//

//================================================================================================//
//                                        FICHIERS INCLUS                                         //
//================================================================================================//

#include "./console.h"

//================================================================================================//
//                                            DEFINES                                             //
//================================================================================================//

//================================================================================================//
//                                          ENUMERATIONS                                          //
//================================================================================================//

//================================================================================================//
//                                      STRUCTURES ET UNIONS                                      //
//================================================================================================//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 VARIABLES PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
static e_CONSOLE _e_console = CONSOLE_READ_LINE;
static String console_buffer = "";
static String lastcommand = "";
static int16_t flag;
static int16_t col;
//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//
float softVersion = 0.0;

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//
static void _PrintBanner(void)
{
    char versionString[5];
    uint8_t minorVersion;

    versionString[0] = (char)(softVersion) + '0';
    versionString[1] = '.';
    minorVersion = (uint8_t)((softVersion + 0.001) * 100.);
    versionString[2] = (char)(minorVersion / 10) + '0';
    versionString[3] = (char)(minorVersion % 10) + '0';
    versionString[4] = '\0';
    SERIAL_PrintString("\r\n\r\n\r\nK-Boot v");
    SERIAL_PrintString(String(versionString));
    SERIAL_PrintString(" \r\n");
}

static void _delete_char(void)
{
    if (col > 0)
    {
        if (console_buffer.endsWith("\t"))
        {
            for (int i = 0; i < 8; i++)
            {
                SERIAL_PrintString("\b \b");
            }
            col -= 8;
        }
        else
        {
            SERIAL_PrintString("\b \b");
            col--;
        }
    }
}

static int16_t _readline(void)
{
    int16_t finish = -1;
    if (SERIAL_Available())
    {
        uint8_t c = SERIAL_Getc();
        /*
         * Special character handling
         */
        switch (c)
        {
        case '\r': /* Enter        */
        case '\n':
            console_buffer += '\0';
            finish = console_buffer.length();
            SERIAL_PrintString("\r\n");
            break;
        case '\0': /* nul            */
            break;
        case 0x03: /* ^C - break        */
            console_buffer = "";
            break;
        case 0x15: /* ^U - erase line    */
            for (int i = 0; i < col; i++)
            {
                SERIAL_PrintString("\b \b");
            }
            console_buffer = "";
            col = 0;
            break;
        case 0x17: /* ^W - erase word    */
            _delete_char();

            break;
        case 0x08:
        case 0x7F:
            _delete_char();
            break;
        default:
            /*
             * Must be a normal character then
             */
            if (c == '\t')
            {
                console_buffer += "\t";
                SERIAL_PrintString("        ");
                col += 8;
            }
            else
            {
                console_buffer += c;
                SERIAL_PrintC(c);
                col++;
            }
            break;
        }
    }

    return finish;
}

//-----------------------------------------------------------------------------
// FONCTION    : _parse_line
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static int16_t _parse_line(char *line, char *argv[])
{
    int16_t nargs = 0;
    while (nargs < CONFIG_SYS_MAXARGS)
    {
        /* skip any white space */
        while (isblank(*line))
            ++line;

        if (*line == '\0')
        { /* end of line, no more args    */
            argv[nargs] = NULL;
            return (nargs);
        }

        argv[nargs++] = line; /* begin of argument string    */

        /* find end of string */
        while (*line && !isblank(*line))
            ++line;

        if (*line == '\0')
        { /* end of line, no more args    */
            argv[nargs] = NULL;
            return (nargs);
        }

        *line++ = '\0'; /* terminate current arg     */
    }

    return (nargs);
}

//-----------------------------------------------------------------------------
// FONCTION    : _process_macros
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static void _process_macros (const char *input, char *output)
{
   char c, prev;
   const char *varname_start = NULL;
   int16_t inputcnt = strlen(input);
   int16_t outputcnt = 64;
   int16_t state = 0;        /* 0 = waiting for '$'  */
   int16_t i;
   char envname[64], *envval;
   int16_t envcnt;    /* Varname # of chars */
 
   /* 1 = waiting for '(' or '{' */
   /* 2 = waiting for ')' or '}' */
   /* 3 = waiting for '''  */
   prev = '\0';        /* previous character   */
 
   while (inputcnt && outputcnt)
   {
      c = *input++;
      inputcnt--;
 
      if (state != 3)
      {
         /* remove one level of escape characters */
         if ((c == '\\') && (prev != '\\'))
         {
            if (inputcnt-- == 0)
               break;
            prev = c;
            c = *input++;
         }
      }
 
      switch (state)
      {
         case 0:    /* Waiting for (unescaped) $    */
            if ((c == '\'') && (prev != '\\'))
            {
               state = 3;
               break;
            }
            if ((c == '$') && (prev != '\\'))
            {
               state++;
            }
            else
            {
               *(output++) = c;
               outputcnt--;
            }
            break;
            case 1:    /* Waiting for (        */
               if (c == '(' || c == '{')
               {
                  state++;
                  varname_start = input;
               }
               else
               {
                  state = 0;
                  *(output++) = '$';
                  outputcnt--;
 
                  if (outputcnt)
                  {
                     *(output++) = c;
                     outputcnt--;
                  }
               }
               break;
         case 2:    /* Waiting for )        */
            if (c == ')' || c == '}')
            {
               envcnt = input - varname_start - 1;    /* Varname # of chars */
 
               /* Get the varname */
               for (i = 0; i < envcnt; i++)
               {
                  envname[i] = varname_start[i];
               }
               envname[i] = 0;
 
               /* Get its value */
               envval = ""; //--- ENV_getenv((UINT8 *)envname);
 
               /* Copy into the line if it exists */
               if (envval != NULL)
               {
                  while ((*envval) && outputcnt)
                  {
                     *(output++) = *(envval++);
                     outputcnt--;
                  }
               }
               /* Look for another '$' */
               state = 0;
            }
            break;
         case 3:    /* Waiting for '        */
            if ((c == '\'') && (prev != '\\'))
            {
               state = 0;
            }
            else
            {
               *(output++) = c;
               outputcnt--;
            }
            break;
      }
      prev = c;
   }
 
   if (outputcnt)
      *output = 0;
   else
      *(output - 1) = 0;
}

//-----------------------------------------------------------------------------
// FONCTION    : _builtin_run_command
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static int16_t _builtin_run_command(String cmd, int16_t flag)
{
    char cmdbuf[64];
    char finaltoken[64];
    char *str = cmdbuf;
    char *argv[CONFIG_SYS_MAXARGS + 1]; /* NULL terminated    */
    char *token;                        /* start of token in cmdbuf    */
    char *sep;                          /* end of token (separator) in cmdbuf */
    int16_t argc, inquotes;
    int16_t repeatable = 1;
    int16_t rc = 0;

    if (cmd.length() == 0)
    {
        return -1; /* empty command */
    }

    strcpy(cmdbuf, cmd.c_str());

    while (*str)
    {
        /*
         * Find separator, or string end
         * Allow simple escape of ';' by writing "\;"
         */
        for (inquotes = 0, sep = str; *sep; sep++)
        {
            if ((*sep == '\'') && (*(sep - 1) != '\\'))
                inquotes = !inquotes;

            if (!inquotes &&
                (*sep == ';') &&      /* separator        */
                (sep != str) &&       /* past string start    */
                (*(sep - 1) != '\\')) /* and NOT escaped    */
                break;
        }

        /*
         * Limit the token to data between separators
         */
        token = str;
        if (*sep)
        {
            str = sep + 1; /* start of command for next pass */
            *sep = '\0';
        }
        else
            str = sep; /* no more commands for next pass */

        /* find macros in this token and replace them */
        _process_macros(token, finaltoken);

        /* Extract arguments */
        if ((argc = _parse_line(finaltoken, argv)) == 0)
        {
            rc = -1; /* no command at all */
            continue;
        }

        if (CMD_process(flag, argc, argv, &repeatable))
            rc = -1;
    }
}

//-----------------------------------------------------------------------------
// FONCTION    : _run_command
//
// DESCRIPTION :
//-----------------------------------------------------------------------------
static int16_t _run_command(String cmd, int16_t flag)
{
    /*
     * builtin_run_command can return 0 or 1 for success, so clean up
     * its result.
     */
    if (_builtin_run_command(cmd, flag) == -1)
        return 1;

    return 0;
}

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : CONSOLE_TaskInit
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
bool CONSOLE_TaskInit(void)
{
    console_buffer = "";
    _e_console = CONSOLE_DISPLAY_PROMPT;
    _PrintBanner();
    return true;
}

void CONSOLE_TaskRun(void)
{
    int16_t rc = 1;
    /*
     * Main Loop for Monitor Command Processing
     */
    switch (_e_console)
    {
    case CONSOLE_DISPLAY_PROMPT:
    {
        SERIAL_PrintString("Yaus-Boot>");
        _e_console = CONSOLE_READ_LINE;
        col = 0;
        break;
    }
    case CONSOLE_READ_LINE:
    {
        int16_t len = _readline();

        flag = 0; /* assume no special flags for now */
        if (len > 0)
        {
            lastcommand = console_buffer;
        }
        else if (len == 0)
        {
            /* cmd Repeat */
            flag |= CMD_FLAG_REPEAT;
        }

        if (len != -1)
        {
            _e_console = CONSOLE_EXECUTE_CMD;
        }

        break;
    }
    case CONSOLE_EXECUTE_CMD:
    {
        rc = _run_command(lastcommand, flag);
        if (rc <= 0)
        {
            /* invalid command or not repeatable, forget it */
            lastcommand = "";
        }
        _e_console = CONSOLE_DISPLAY_PROMPT;
        break;
    }
    }
}

#if 0
void CONSOLE_TaskRun(void)
{
    switch (_e_console)
    {
    case (CONSOLE_BOOTING):
    {
        Serial.println(diybms_logo);
        Serial.print(F("Press SPACE BAR to enter terminal based configuration...."));
        _e_console = CONSOLE_WAIT_SPACE;
        _ms2 = millis();
        break;
    }
    case (CONSOLE_WAIT_SPACE):
    {
        if ((millis() - _ms2) > 3000)
        {
            Serial.println(F("skipped"));
            Serial.flush();
            _e_console = CONSOLE_IDLE;
            break;
        }
        if ((millis() - _ms) > 250)
        {
            Serial.print('.');
            _ms = millis();
        }
        if (Serial.available())
        {
            int x = Serial.read();
            // SPACE BAR
            if (x == 32)
            {
                _e_console = CONSOLE_WIFI_SETTINGS;
                Serial.println(F("Enter Terminal Mode"));
                Serial.flush();
                break;
            }
        }
        break;
    }
  /*  case (CONSOLE_TERMINAL_MODE):
    {
        Serial.flush();
        Serial.println(F("\r\n\r\nDIYBMS CONTROLLER - Console Menu"));
        Serial.println(F("1 : Wifi Settings"));
        Serial.println(F("2 : Upload File"));
        Serial.println();
        Serial.print(F("Enter the NUMBER :"));
        _e_console = CONSOLE_WAITING_COMMAND;
        while (Serial.available())
        {
            Serial.read();
        }
        break;
    }
    case (CONSOLE_WAITING_COMMAND):
    {
        if (Serial.available())
        {
            char x = Serial.read();
            Serial.print(x);
            _cmd += String(x);
            if (_cmd == "1")
            {
                _cmd = "";
                _e_console = CONSOLE_WIFI_SETTINGS;
                Serial.flush();
            }
            else
            {
                Serial.print(F("ERROR !!"));
                _cmd = "";
                _e_console = CONSOLE_WAITING_COMMAND;
            }
        }
        break;
    }*/
    case (CONSOLE_WIFI_SETTINGS):
    {
        Serial.println(F("\r\n\r\nDIYBMS CONTROLLER - Scanning Wifi"));

        WiFi.mode(WIFI_STA);
        WiFi.setSleep(false);
        WiFi.disconnect();

        int n = WiFi.scanNetworks();

        if (n == 0)
            Serial.println(F("no networks found"));
        else
        {
            for (int i = 0; i < n; ++i)
            {
                if (i < 10)
                {
                    Serial.print(' ');
                }
                Serial.print(i);
                Serial.print(':');
                Serial.print(WiFi.SSID(i));

                // Pad out the wifi names into 2 columns
                for (size_t spaces = WiFi.SSID(i).length(); spaces < 36; spaces++)
                {
                    Serial.print(' ');
                }

                if ((i + 1) % 2 == 0)
                {
                    Serial.println();
                }
                delay(5);
            }
            Serial.println();
        }

        WiFi.mode(WIFI_OFF);

        Serial.print(F("Enter the NUMBER of the Wifi network to connect to:"));

        bool result;
        char buffer[10];
        result = CaptureSerialInput(Serial, buffer, 10, true, false);
        if (result)
        {
            int index = String(buffer).toInt();
            Serial.print(F("Enter the password to use when connecting to '"));
            Serial.print(WiFi.SSID(index));
            Serial.print("':");

            char passwordbuffer[80];
            result = CaptureSerialInput(Serial, passwordbuffer, 80, false, true);

            if (result)
            {
                diybms_eeprom_settings *_settings = SETTINGS_Get();
                _settings->wifi_ssid = WiFi.SSID(index);
                _settings->wifi_psk = String(passwordbuffer);
                SETTINGS_Save();
            }
        }
        Serial.println(F("REBOOTING IN 5..."));
        delay(5000);
        ESP.restart();
        break;
    }
 /*   case (CONSOLE_UPLOAD_FILE):
    {
        Serial.println();
        Serial.print(F("Enter file's name ( < 64 chars ): "));
        bool result;
        char buffer[64];
        result = CaptureSerialInput(Serial, buffer, 64, false, false);
        while (Serial.available())
        {
            Serial.read();
        }
        if (result)
        {
            SDFS *SD = SDCARD_GetSD();

            if (SD->exists(buffer))
            {
                SD->remove(buffer);
            }

            Serial.print(F("Drag and drop the file in console "));

            _ms2 = millis() + 15000;

            while (Serial.available())
            {
                Serial.read();
            }

            while (true)
            {
                if (Serial.available() == true)
                {
                    break;
                }
                if (millis() > _ms2)
                {
                    Serial.print(F("ERROR !!"));
                    _e_console = CONSOLE_TERMINAL_MODE;
                    Serial.flush();
                    return;
                }
            }
            File _f = SD->open(buffer, "w");

            _ms2 = millis() + 500;

            while (true)
            {
                char x;
                if (Serial.available())
                {
                    x = (char)Serial.read();
                    Serial.print(x);
                    _f.write(x);
                    _ms2 = millis() + 500;
                }
                if (millis() > _ms2)
                {
                    _f.close();
                    break;
                }
            }
        }
        else
        {
            Serial.print(F("ERROR !!"));
            _e_console = CONSOLE_TERMINAL_MODE;
        }
        break;
    }*/
    case (CONSOLE_IDLE):
    {
        break;
    }
    }
}
#endif