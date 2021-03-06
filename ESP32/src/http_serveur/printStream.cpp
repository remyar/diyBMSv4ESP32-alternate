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

#include "./printStream.h"

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

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 FONCTIONS PRIVEES ET PARTAGEES                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------------------------//
//---                                         Privees                                          ---//
//------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------//
//---                                        Partagees                                         ---//
//------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------
// FONCTION    : MONITOR2_JSON
//
// DESCRIPTION : Initialisation de la carte : GPIO, Clocks, Interruptions...
//--------------------------------------------------------------------------------------------------
void PrintStreamCommaBoolean(AsyncResponseStream *response, const char *text, bool boolean)
{
  response->print(text);
  if (boolean)
  {
    response->print("true");
  }
  else
  {
    response->print("false");
  }
  response->print(',');
}

void PrintStreamCommaFloat(AsyncResponseStream *response, const char *text, float value)
{
  response->print(text);
  //Print value to 6 decimal places
  response->print(value, 6);
  response->print(',');
}

void PrintStreamComma(AsyncResponseStream *response, const char *text, uint32_t value)
{
  response->print(text);
  response->print(value);
  response->print(',');
}

void PrintStreamCommaInt16(AsyncResponseStream *response, const char *text, int16_t value)
{
  response->print(text);
  response->print(value);
  response->print(',');
}

void PrintStream(AsyncResponseStream *response, const char *text, uint32_t value)
{
  response->print(text);
  response->print(value);
}