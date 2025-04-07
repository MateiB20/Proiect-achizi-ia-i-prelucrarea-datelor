/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_BINARYSWITCH_2             2       /* control type: binary, callback function: OnSwitch */
#define  PANEL_GRAPH_RAW_DATA             3       /* control type: graph, callback function: OnLoadButtonCB */
#define  PANEL_COMMANDBUTTON              4       /* control type: command, callback function: Command */
#define  PANEL_NUMERIC_14                 5       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_13                 6       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_9                  7       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_10                 8       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_8                  9       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_5                  10      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_7                  11      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_6                  12      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_3                  13      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_12                 14      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_11                 15      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_4                  16      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_2                  17      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_1                  18      /* control type: numeric, callback function: (none) */
#define  PANEL_FILTER_1                   19      /* control type: graph, callback function: (none) */
#define  PANEL_HISTO                      20      /* control type: graph, callback function: (none) */
#define  PANEL_RINGSLIDE                  21      /* control type: slide, callback function: (none) */
#define  PANEL_COMMANDBUTTON_PREV         22      /* control type: command, callback function: Prev */
#define  PANEL_COMMANDBUTTON_NEXT         23      /* control type: command, callback function: Next */
#define  PANEL_BINARYSWITCH               24      /* control type: binary, callback function: (none) */

#define  PANEL_2                          2
#define  PANEL_2_BINARYSWITCH             2       /* control type: binary, callback function: OnSwitch_2 */
#define  PANEL_2_GRAPH_RAW_DATA           3       /* control type: graph, callback function: OnLoadButtonCB */
#define  PANEL_2_COMMANDBUTTON            4       /* control type: command, callback function: Command */
#define  PANEL_2_NUMERIC_9                5       /* control type: numeric, callback function: (none) */
#define  PANEL_2_TIMER                    6       /* control type: timer, callback function: OnTimer */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Command(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Next(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitch_2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Prev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif