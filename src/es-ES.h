////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2006-2013, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_STRINGS
#define INCLUDED_STRINGS

// columns/Col*
#define STRING_COLUMN_LABEL_TASKS    "Tareas"
#define STRING_COLUMN_LABEL_DESC     "Descripción"
#define STRING_COLUMN_LABEL_DUE      "Vencimiento"
#define STRING_COLUMN_LABEL_ENTERED  "Entrada"
#define STRING_COLUMN_LABEL_COUNT    "Recuento"
#define STRING_COLUMN_LABEL_PROJECT  "Proyecto"
#define STRING_COLUMN_LABEL_STAT     "Est"
#define STRING_COLUMN_LABEL_STAT_PE  "Pendientes"
#define STRING_COLUMN_LABEL_STAT_CO  "Completadas"
#define STRING_COLUMN_LABEL_STAT_DE  "Suprimidas"
#define STRING_COLUMN_LABEL_STAT_WA  "Esperando"
// Mejor Periódica, pero STRING_COLUMN_LABEL_STAT_P es Pendiente
#define STRING_COLUMN_LABEL_STAT_RE  "Recurrentes"
#define STRING_COLUMN_LABEL_STAT_P   "P"
#define STRING_COLUMN_LABEL_STAT_C   "C"
#define STRING_COLUMN_LABEL_STAT_D   "S"
#define STRING_COLUMN_LABEL_STAT_W   "E"
#define STRING_COLUMN_LABEL_STAT_R   "R"
#define STRING_COLUMN_LABEL_TAGS     "Marcas"
#define STRING_COLUMN_LABEL_TAG      "Marca"
#define STRING_COLUMN_LABEL_UUID     "UUID"
#define STRING_COLUMN_LABEL_URGENCY  "Urgencia"
#define STRING_COLUMN_LABEL_VALUE    "Valor"
#define STRING_COLUMN_LABEL_DATE     "Fecha"
#define STRING_COLUMN_LABEL_SCHED    "Programada"
#define STRING_COLUMN_LABEL_UDA      "Nombre"
#define STRING_COLUMN_LABEL_TYPE     "Tipo"
#define STRING_COLUMN_LABEL_LABEL    "Etiqueta"
#define STRING_COLUMN_LABEL_DEFAULT  "Defecto"
#define STRING_COLUMN_LABEL_VALUES   "Valores permitidos"
#define STRING_COLUMN_LABEL_UDACOUNT "Recuento de uso"
#define STRING_COLUMN_LABEL_ORPHAN   "UDA huérfano"

// commands/Cmd*
#define STRING_CMD_UDAS_USAGE        "Muestra detalles de todos los UDA definidos"
#define STRING_CMD_UDAS_COMPL_USAGE  "Muestra los UDAs definidos con fines de terminación"
// metadatos: RAE 3ªed
#define STRING_CMD_INFO_BLOCKED      "Esta tarea está bloqueada por"
#define STRING_CMD_INFO_BLOCKING     "Esta tarea bloquea"
#define STRING_CMD_INFO_MODIFICATION "Modificación"
#define STRING_CMD_INFO_MODIFIED     "Modificada por última vez"
#define STRING_CMD_STATS_CATEGORY    "Categoría"
#define STRING_CMD_STATS_DATA        "Datos"
#define STRING_CMD_STATS_TOTAL       "Total"
#define STRING_CMD_STATS_ANNOTATIONS "Anotaciones"
#define STRING_CMD_STATS_UNIQUE_TAGS "Marcas únicas"
#define STRING_CMD_STATS_PROJECTS    "Proyectos"
#define STRING_CMD_STATS_DATA_SIZE   "Tamaño de datos"
#define STRING_CMD_STATS_UNDO_TXNS   "Transacciones deshacer"
#define STRING_CMD_STATS_BACKLOG     "Sincroniza transacciones atrasadas"
#define STRING_CMD_STATS_TAGGED      "Tareas marcadas"
#define STRING_CMD_STATS_OLDEST      "Tarea más antigua"
#define STRING_CMD_STATS_NEWEST      "Tarea más reciente"
//#define STRING_CMD_STATS_USED_FOR    "Tarea usada por"
#define STRING_CMD_STATS_USED_FOR    "Tarea en uso durante"
#define STRING_CMD_STATS_ADD_EVERY   "Tarea añadida cada"
#define STRING_CMD_STATS_COMP_EVERY  "Tarea completada cada"
#define STRING_CMD_STATS_DEL_EVERY   "Tarea suprimida cada"
#define STRING_CMD_STATS_AVG_PEND    "Tiempo medio pendiente"

#define STRING_CMD_STATS_DESC_LEN    "Longitud media de desc"
#define STRING_CMD_STATS_CHARS       "{1} caracteres"
#define STRING_CMD_STATS_LAST_SYNC   "Ültima sincronización del servidor"
#define STRING_CMD_STATS_BLOCKED     "Tareas bloqueadas"
#define STRING_CMD_STATS_BLOCKING    "Tareas bloqueantes"
#define STRING_CMD_TAGS_SINGLE       "1 marca"
#define STRING_CMD_TAGS_PLURAL       "{1} marcas"

#define STRING_CMD_HISTORY_YEAR      "Año"
#define STRING_CMD_HISTORY_MONTH     "Mes"
#define STRING_CMD_HISTORY_ADDED     "Añadidas"
#define STRING_CMD_HISTORY_COMP      "Completadas"
#define STRING_CMD_HISTORY_DEL       "Suprimidas"
#define STRING_CMD_HISTORY_NET       "Netas"
#define STRING_CMD_HISTORY_LEGEND    "Leyenda: {1}, {2}, {3}"

#define STRING_CMD_GHISTORY_YEAR     "Año"
#define STRING_CMD_GHISTORY_MONTH    "Mes"
#define STRING_CMD_GHISTORY_NUMBER   "Número Añadidas/Completadas/Suprimidas"

#define STRING_CMD_DONE_NO           "Tarea no completada."
#define STRING_CMD_DONE_NOTPEND      "La tarea {1} '{2}' no está pendiente ni esperando."
#define STRING_CMD_DONE_1            "Completada {1} tarea."
#define STRING_CMD_DONE_N            "Completadas {1} tareas."

#define STRING_CMD_PROJECTS_PRI_N    "Pri:None"
#define STRING_CMD_PROJECTS_PRI_H    "Pri:H"
#define STRING_CMD_PROJECTS_PRI_M    "Pri:M"
#define STRING_CMD_PROJECTS_PRI_L    "Pri:L"
#define STRING_CMD_PROJECTS_SUMMARY  "{1} proyecto"
#define STRING_CMD_PROJECTS_SUMMARY2 "{1} proyectos"
#define STRING_CMD_PROJECTS_TASK     "({1} tarea)"
#define STRING_CMD_PROJECTS_TASKS    "({1} tareas)"
#define STRING_CMD_SUMMARY_PROJECT   "Proyecto"
#define STRING_CMD_SUMMARY_REMAINING "Quedan"
#define STRING_CMD_SUMMARY_AVG_AGE   "Edad media"
#define STRING_CMD_SUMMARY_COMPLETE  "Completas"

#define STRING_CMD_UDAS_NO           "Ningún UDA definido."
#define STRING_CMD_UDAS_SUMMARY      "{1} UDA definido"
#define STRING_CMD_UDAS_SUMMARY2     "{1} UDAs definidos"
#define STRING_CMD_UDAS_ORPHAN       "{1} UDA huérfano"
#define STRING_CMD_UDAS_ORPHANS      "{1} UDAs huérfanos"

#define STRING_CMD_DUPLICATE_1       "Duplicada {1} tarea."
#define STRING_CMD_DUPLICATE_N       "Duplicadas {1} tareas."

#define STRING_CMD_PREPEND_1         "Antepuesto texto a {1} tarea."
#define STRING_CMD_PREPEND_N         "Antepuesto texto a {1} tareas."

#define STRING_CMD_DENO_NONE         "La tarea especificada no tiene anotaciones que puedan ser eliminadas."
#define STRING_CMD_DENO_FOUND        "Encontrada anotación '{1}' y eliminada."
#define STRING_CMD_DENO_NOMATCH      "No se encontró ninguna anotación coincidente para eliminar en '{1}'."
#define STRING_CMD_DENO_NO           "Tarea no desanotada."
#define STRING_CMD_DENO_1            "Desanotada {1} tarea."
#define STRING_CMD_DENO_N            "Desanotadas {1} tareas."


#define STRING_CMD_SYNC_SUCCESS0     "Sincronización correcta."
#define STRING_CMD_SYNC_SUCCESS1     "Sincronización correcta. {1} cambios enviados."
#define STRING_CMD_SYNC_SUCCESS2     "Sincronización correcta. {1} cambios recibidos."
#define STRING_CMD_SYNC_SUCCESS3     "Sincronización correcta. {1} cambios enviados, {2} cambios recibidos."
#define STRING_CMD_SYNC_SUCCESS_NOP  "Sincronización correcta. No hay cambios"
#define STRING_CMD_SYNC_FAIL_ACCOUNT "Sincronización fallida. O bien sus credenciales son incorrectas, o su cuenta de Servidor Task no está habilitada."
#define STRING_CMD_SYNC_FAIL_ERROR   "Sincronización fallida. El Servidor Task devolvió error: {1} {2}"
#define STRING_CMD_SYNC_FAIL_CONNECT "Sincronización fallida. No se pudo conectar con el Servidor Task."
#define STRING_CMD_SYNC_BAD_SERVER   "Sincronización fallida. Ajuste de configuración '{1}' incorrecto"
#define STRING_CMD_SYNC_NO_TLS       "Taskwarrior was built without GnuTLS support.  Sync is not available."

#define STRING_CMD_DIAG_UNKNOWN      "<desconocido>"
#define STRING_CMD_DIAG_COMPILER     "Compilador"
#define STRING_CMD_DIAG_VERSION      "Versión"
#define STRING_CMD_DIAG_CAPS         "Capacidades"
#define STRING_CMD_DIAG_FEATURES     "Características Constructivas"
#define STRING_CMD_DIAG_BUILT        "Construido"
#define STRING_CMD_DIAG_COMMIT       "Commit"
#define STRING_CMD_DIAG_UUID_SCAN    "Exploradas {1} tareas buscando UUIDs duplicados:"
#define STRING_CMD_DIAG_UUID_DUP     "Encontrado duplicado {1}"
#define STRING_CMD_DIAG_UUID_NO_DUP  "No se encontraron duplicados"

#define STRING_CMD_MODIFY_NO_DUE     "No puede especificar una tarea recurrente sin una fecha de vencimiento."
#define STRING_CMD_MODIFY_TASK       "Modificando tarea {1} '{2}'."
#define STRING_CMD_MODIFY_TASK_R     "Modificando tarea recurrente {1} '{2}'."
#define STRING_CMD_MODIFY_1          "Modificada {1} tarea."
#define STRING_CMD_MODIFY_N          "Modificada {1} tareas."
#define STRING_CMD_MODIFY_NO         "Tarea no modificada."

#define STRING_CMD_TIMESHEET_STARTED "Comenzadas ({1} tareas)"
#define STRING_CMD_TIMESHEET_DONE    "Completadas ({1} tareas)"
#define STRING_CMD_BURN_STARTED      "Comenz."             // Must be 7 or fewer characters
#define STRING_CMD_BURN_DONE         "Hechas"              // Must be 7 or fewer characters
#define STRING_CMD_BURN_PENDING      "Pend."               // Must be 7 or fewer characters

// Context
#define STRING_CONTEXT_CREATE_RC     "No se pudo encontrar un archivo de configuración en {1}\n\n¿Desearía que se creara un {2} de ejemplo, para que taskwarrior pueda continuar?"
#define STRING_CONTEXT_NEED_RC       "No se puede continuar sin archivo de configuración (rc)."
#define STRING_CONTEXT_SHADOW_P      "La variable de configuración 'shadow.file' está ajustada a " "sobreescribrir sus tareas pendientes. Por favor, cambie esto."
#define STRING_CONTEXT_SHADOW_C      "La variable de configuración 'shadow.file' está ajustada a " "sobreescribrir sus tareas completadas. Por favor, cambie esto."
#define STRING_CONTEXT_SHADOW_U      "La variable de configuración 'shadow.file' está ajustada a " "sobreescribrir su registro de deshacer. Por favor, cambie esto."
#define STRING_CONTEXT_SHADOW_B      "La variable de configuración 'shadow.file' está ajustada a " "sobreescribrir su archivo backlog. Por favor, cambie esto."
#define STRING_CONTEXT_SHADOW_UPDATE "[Archivo shadow '{1}' actualizado.]"

// Date
#define STRING_DATE_BAD_WEEKSTART    "La variable de configuración 'weekstart' solamente puede contener 'Sunday' (domingo) o 'Monday' (lunes)."

#define STRING_DATE_JANUARY_LONG     "enero"
#define STRING_DATE_FEBRUARY_LONG    "febrero"
#define STRING_DATE_MARCH_LONG       "marzo"
#define STRING_DATE_APRIL_LONG       "abril"
#define STRING_DATE_MAY_LONG         "mayo"
#define STRING_DATE_JUNE_LONG        "junio"
#define STRING_DATE_JULY_LONG        "julio"
#define STRING_DATE_AUGUST_LONG      "agosto"
#define STRING_DATE_SEPTEMBER_LONG   "septiembre"
#define STRING_DATE_OCTOBER_LONG     "octubre"
#define STRING_DATE_NOVEMBER_LONG    "noviembre"
#define STRING_DATE_DECEMBER_LONG    "diciembre"

#define STRING_DATE_JANUARY_SHORT    "ene"
#define STRING_DATE_FEBRUARY_SHORT   "feb"
#define STRING_DATE_MARCH_SHORT      "mar"
#define STRING_DATE_APRIL_SHORT      "abr"
#define STRING_DATE_MAY_SHORT        "may"
#define STRING_DATE_JUNE_SHORT       "jun"
#define STRING_DATE_JULY_SHORT       "jul"
#define STRING_DATE_AUGUST_SHORT     "ago"
#define STRING_DATE_SEPTEMBER_SHORT  "sep"
#define STRING_DATE_OCTOBER_SHORT    "oct"
#define STRING_DATE_NOVEMBER_SHORT   "nov"
#define STRING_DATE_DECEMBER_SHORT   "dic"

#define STRING_DATE_SUNDAY_LONG      "domingo"
#define STRING_DATE_MONDAY_LONG      "lunes"
#define STRING_DATE_TUESDAY_LONG     "martes"
#define STRING_DATE_WEDNESDAY_LONG   "miércoles"
#define STRING_DATE_THURSDAY_LONG    "jueves"
#define STRING_DATE_FRIDAY_LONG      "viernes"
#define STRING_DATE_SATURDAY_LONG    "sábado"

#define STRING_DATE_SUNDAY_SHORT     "dom"
#define STRING_DATE_MONDAY_SHORT     "lun"
#define STRING_DATE_TUESDAY_SHORT    "mar"
#define STRING_DATE_WEDNESDAY_SHORT  "mie"
#define STRING_DATE_THURSDAY_SHORT   "jue"
#define STRING_DATE_FRIDAY_SHORT     "vie"
#define STRING_DATE_SATURDAY_SHORT   "sab"

// edit
#define STRING_EDIT_END              "Fin"

#define STRING_EDIT_END_MOD          "Fecha de terminación modificada."
#define STRING_EDIT_END_DEL          "Fecha de terminación eliminada."
#define STRING_EDIT_END_SET_ERR      "No se puede establecer una fecha de completado en una tarea pendiente."
#define STRING_EDIT_UDA_MOD          "UDA {1} modificado."
#define STRING_EDIT_UDA_DEL          "UDA {1} eliminado."

// These four blocks can be replaced, but the number of lines must not change.
#define STRING_EDIT_HEADER_1         "El comando 'task <id> edit' le permite modificar todos los detalles de"
#define STRING_EDIT_HEADER_2         "una tarea usando un editor de textos. Más abajo hay una representación"
#define STRING_EDIT_HEADER_3         "de todos los detalles de la tarea. Modifique lo que desee y, cuando grabe"
#define STRING_EDIT_HEADER_4         "y salga del editor, taskwarrior leerá este archivo, determinará qué ha"
#define STRING_EDIT_HEADER_5         "cambiado, y aplicará esos cambios. Si sale del editor sin grabar o sin"
#define STRING_EDIT_HEADER_6         "hacer modificaciones, taskwarrior no hará nada."

#define STRING_EDIT_HEADER_7         "Las líneas que comienzan con # representan datos que no puede cambiar, como ID."
#define STRING_EDIT_HEADER_8         "Si es demasiado creativo con sus modificaciones, taskwarrior le enviará de vuelta"
#define STRING_EDIT_HEADER_9         "al editor para que lo intente de nuevo."

#define STRING_EDIT_HEADER_10        "Si queda atrapado en un ciclo sin fin, volviendo a editar el mismo archivo"
#define STRING_EDIT_HEADER_11        "una y otra vez, simplemente abandone el editor sin hacer ningún cambio."
#define STRING_EDIT_HEADER_12        "Taskwarrior lo advertirá y detendrá el proceso."

// Maintain the same spacing.
#define STRING_EDIT_TABLE_HEADER_1   "Nombre             Detalles modificables"
#define STRING_EDIT_TABLE_HEADER_2   "-----------------  ----------------------------------------------------"

// Feedback
#define STRING_FEEDBACK_TASKS_SINGLE "(1 tarea)"
#define STRING_FEEDBACK_TASKS_PLURAL "({1} tareas)"
#define STRING_FEEDBACK_DELETED      "Se eliminará {1}."
#define STRING_FEEDBACK_DEP_SET      "Las dependencias se ajustarán a '{1}'."
#define STRING_FEEDBACK_DEP_MOD      "Las dependencias se cambiarán de '{1}' a '{2}'."
#define STRING_FEEDBACK_DEP_DEL      "Dependencias '{1}' eliminadas."
#define STRING_FEEDBACK_DEP_WAS_SET  "Dependencias ajustadas a '{1}'."
#define STRING_FEEDBACK_DEP_WAS_MOD  "Dependencias cambiadas de '{1}' a '{2}'."
#define STRING_FEEDBACK_ATT_SET      "{1} se establecerá como '{2}'."
#define STRING_FEEDBACK_ATT_MOD      "{1} se cambiará de '{2}' a '{3}'."
#define STRING_FEEDBACK_ATT_DEL      "{1} eliminado."
#define STRING_FEEDBACK_ATT_DEL_DUR  "{1} eliminado (duración: {2})."
#define STRING_FEEDBACK_ATT_WAS_SET  "{1} establecido como '{2}'."
#define STRING_FEEDBACK_ATT_WAS_MOD  "{1} cambiado de '{2}' a '{3}'."
#define STRING_FEEDBACK_ANN_ADD      "Anotación de '{1}' añadida."
#define STRING_FEEDBACK_ANN_DEL      "Anotación '{1}' eliminada."
#define STRING_FEEDBACK_ANN_WAS_MOD  "Anotación cambiada a '{1}'."
#define STRING_FEEDBACK_UNBLOCKED    "Desbloqueada {1} '{2}'."
#define STRING_FEEDBACK_EXPIRED      "La tarea {1} '{2}' caducó y fue eliminada."

// 'show' command
#define STRING_CMD_SHOW              "Muestra todas las variables de configuración o un subconjunto"
#define STRING_CMD_SHOW_UNREC        "Su archivo .taskrc contiene estas variables no reconocidas:"
#define STRING_CMD_SHOW_DIFFER       "Algunas de sus variables .taskrc difieren de los valores por defecto."
#define STRING_CMD_SHOW_DIFFER_COLOR "Las cambiadas aparecen resaltadas en {1} más arriba."

// Task
#define STRING_TASK_VALID_PRIORITY   "Valores de prioridad pueden ser 'H', 'M' o 'L', no '{1}'."

// Taskmod
#define STRING_TASKMOD_TIME          "tiempo "
#define STRING_TASKMOD_OLD           "antiguo "
#define STRING_TASKMOD_NEW           "nuevo "

// TDB2
#define STRING_TDB2_REMOTE_CHANGE    "Encontrado cambio remoto en   {1}  \"{2}\""
#define STRING_TDB2_LOCAL_CHANGE     "Reteniendo cambios locales a  {1}  \"{2}\""
#define STRING_TDB2_MISSING          "Falta                         {1}  \"{2}\""
#define STRING_TDB2_MERGING          "Combinando tarea remota nueva {1}  \"{2}\""
#define STRING_TDB2_LAST_MOD         "La última modificación fue hecha {1}"
#define STRING_TDB2_DIFF_PREV        "--- estado previo "             // Same length
#define STRING_TDB2_DIFF_PREV_DESC   "Deshacer restaurará este estado"//   ||
#define STRING_TDB2_DIFF_CURR        "+++ estado actual "             // Same length
#define STRING_TDB2_DIFF_CURR_DESC   "Cambio hecho {1}"
#define STRING_TDB2_MISSING_UUID     "No se pudo localizar el UUID en la tarea a deshacer."
#define STRING_TDB2_MISSING_TASK     "Tarea con UUID {1} no encontrada en los datos."

// ****************************************************************************************************
// Usage text.  This is an exception, and contains \n characters and formatting.
/* FIXME más abajo 
  "  task project:Casa due.before:hoy          <command> <mods>\n" 
  "  task project:Home due.before:today        <command> <mods>\n" 
 y 
  "  task <filter> <command> project:Casa\n" 
  "  task <filter> <command> +finde +jardín due:mañana\n" 
  "  task <filter> <command> Texto anotación/descripción\n" 
 en vez de: 
  "  task <filter> <command> project:Home\n" 
  "  task <filter> <command> +weekend +garden due:tomorrow\n" 
  "  task <filter> <command> Description/annotation text\n" 
 y 
  "  task add -- project:Casa necesita programación\n" 
  "  task add -- project:Home needs scheduling\n" */

// shell
#define STRING_SHELL_USAGE \
  "Uso: tasksh [<fichero-comandos>]   Ejecuta los comandos task contenidos en <fichero-comandos>, si se indica;\n" \
  "                                   si no, lanza un entorno interactivo de comandos task.\n" \
  "     tasksh --version              Muestra la versión de task.\n" \
  "     tasksh --help                 Muestra esta ayuda.\n"

#define STRING_SHELL_NO_FILE         "El fichero de entrada no existe.\n"

#endif

