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
#define L10N                                           // Localization complete.

// columns/Col*
#define STRING_COLUMN_LABEL_TASKS    "Task"
#define STRING_COLUMN_LABEL_DESC     "Descrizione"
#define STRING_COLUMN_LABEL_DUE      "Scadenza"
#define STRING_COLUMN_LABEL_ENTERED  "Inserito"
#define STRING_COLUMN_LABEL_COUNT    "Conteggio"
#define STRING_COLUMN_LABEL_PROJECT  "Progetto"
#define STRING_COLUMN_LABEL_STAT     "St"
#define STRING_COLUMN_LABEL_STAT_PE  "Da fare"
#define STRING_COLUMN_LABEL_STAT_CO  "Completati"
#define STRING_COLUMN_LABEL_STAT_DE  "Cancellati"
#define STRING_COLUMN_LABEL_STAT_WA  "In attesa"
#define STRING_COLUMN_LABEL_STAT_RE  "Periodicamente"
#define STRING_COLUMN_LABEL_STAT_P   "P"
#define STRING_COLUMN_LABEL_STAT_C   "C"
#define STRING_COLUMN_LABEL_STAT_D   "D"
#define STRING_COLUMN_LABEL_STAT_W   "W"
#define STRING_COLUMN_LABEL_STAT_R   "R"
#define STRING_COLUMN_LABEL_TAGS     "Tags"
#define STRING_COLUMN_LABEL_TAG      "Tag"
#define STRING_COLUMN_LABEL_UUID     "UUID"
#define STRING_COLUMN_LABEL_URGENCY  "Urgenza"
#define STRING_COLUMN_LABEL_VALUE    "Valore"
#define STRING_COLUMN_LABEL_DATE     "Data"
#define STRING_COLUMN_LABEL_SCHED    "Fissato"
#define STRING_COLUMN_LABEL_UDA      "Nome"
#define STRING_COLUMN_LABEL_TYPE     "Tipo"
#define STRING_COLUMN_LABEL_LABEL    "Etichetta"
#define STRING_COLUMN_LABEL_DEFAULT  "Predefinito"
#define STRING_COLUMN_LABEL_VALUES   "Valori consentiti"
#define STRING_COLUMN_LABEL_UDACOUNT "Conteggio Uso"
#define STRING_COLUMN_LABEL_ORPHAN   "UDA Orfano"

// commands/Cmd*
#define STRING_CMD_UDAS_USAGE        "Mostra tutti i dettagli degli UDA definiti"
#define STRING_CMD_UDAS_COMPL_USAGE  "Mostra gli UDA definiti per il completamento"
#define STRING_CMD_INFO_BLOCKED      "Questo task è bloccato da"
#define STRING_CMD_INFO_BLOCKING     "Questo task sta bloccando"
#define STRING_CMD_INFO_MODIFICATION "Modifica"
#define STRING_CMD_INFO_MODIFIED     "Ultima modifica"
#define STRING_CMD_STATS_CATEGORY    "Categoria"
#define STRING_CMD_STATS_DATA        "Dati"
#define STRING_CMD_STATS_TOTAL       "Totale"
#define STRING_CMD_STATS_ANNOTATIONS "Annotazioni"
#define STRING_CMD_STATS_UNIQUE_TAGS "Tag univoci"
#define STRING_CMD_STATS_PROJECTS    "Progetti"
#define STRING_CMD_STATS_DATA_SIZE   "Dimensione dati"
#define STRING_CMD_STATS_UNDO_TXNS   "Transazioni di undo"
#define STRING_CMD_STATS_BACKLOG     "Sincronizza le transazioni di backlog"
#define STRING_CMD_STATS_TAGGED      "Task taggati"
#define STRING_CMD_STATS_OLDEST      "Task più vecchio"
#define STRING_CMD_STATS_NEWEST      "Task più recente"
#define STRING_CMD_STATS_USED_FOR    "Task usati per"
#define STRING_CMD_STATS_ADD_EVERY   "Task aggiunti ogni"
#define STRING_CMD_STATS_COMP_EVERY  "Task completati ogni"
#define STRING_CMD_STATS_DEL_EVERY   "Task cancellati ogni"
#define STRING_CMD_STATS_AVG_PEND    "Tempo medio di attesa"
#define STRING_CMD_STATS_DESC_LEN    "Lunghezza media delle descrizioni"
#define STRING_CMD_STATS_CHARS       "{1} caratteri"
#define STRING_CMD_STATS_LAST_SYNC   "Ultima sincronizzazione"
#define STRING_CMD_STATS_BLOCKED     "Task bloccati"
#define STRING_CMD_STATS_BLOCKING    "Task bloccanti"
#define STRING_CMD_TAGS_SINGLE       "1 tag"
#define STRING_CMD_TAGS_PLURAL       "{1} tag"
#define STRING_CMD_HISTORY_YEAR      "Anno"
#define STRING_CMD_HISTORY_MONTH     "Mese"
#define STRING_CMD_HISTORY_ADDED     "Aggiunto"
#define STRING_CMD_HISTORY_COMP      "Completato"
#define STRING_CMD_HISTORY_DEL       "Cancellato"
#define STRING_CMD_HISTORY_NET       "Bilancio"
#define STRING_CMD_HISTORY_LEGEND    "Legenda: {1}, {2}, {3}"
#define STRING_CMD_GHISTORY_YEAR     "Anno"
#define STRING_CMD_GHISTORY_MONTH    "Mese"
#define STRING_CMD_GHISTORY_NUMBER   "Aggiunti/Completati/Cancellati"

#define STRING_CMD_DONE_NO           "Task non completato."
#define STRING_CMD_DONE_NOTPEND      "Il task {1} '{2}' non è ne' pendente ne' in attesa."
#define STRING_CMD_DONE_1            "Completato {1} task."
#define STRING_CMD_DONE_N            "Completati {1} task."

#define STRING_CMD_PROJECTS_PRI_N    "Pri:Nessuna"
#define STRING_CMD_PROJECTS_PRI_H    "Pri:H"
#define STRING_CMD_PROJECTS_PRI_M    "Pri:M"
#define STRING_CMD_PROJECTS_PRI_L    "Pri:L"
#define STRING_CMD_PROJECTS_SUMMARY  "{1} progetto"
#define STRING_CMD_PROJECTS_SUMMARY2 "{1} progetti"
#define STRING_CMD_PROJECTS_TASK     "({1} task)"
#define STRING_CMD_PROJECTS_TASKS    "({1} task)"
#define STRING_CMD_SUMMARY_PROJECT   "Progetto"
#define STRING_CMD_SUMMARY_REMAINING "Rimanenti"
#define STRING_CMD_SUMMARY_AVG_AGE   "Età media"
#define STRING_CMD_SUMMARY_COMPLETE  "Completi"

#define STRING_CMD_UDAS_NO           "Nessun UDA definito."
#define STRING_CMD_UDAS_SUMMARY      "{1} UDA definito"
#define STRING_CMD_UDAS_SUMMARY2     "{1} UDAs defined"
#define STRING_CMD_UDAS_ORPHAN       "{1} UDA orfano"
#define STRING_CMD_UDAS_ORPHANS      "{1} UDA orfani"

#define STRING_CMD_DUPLICATE_1       "Task {1} duplicato."
#define STRING_CMD_DUPLICATE_N       "Task {1} duplicati."

#define STRING_CMD_PREPEND_1         "Aggiunta al task {1}."
#define STRING_CMD_PREPEND_N         "Aggiunte ai task {1}."

#define STRING_CMD_DENO_NONE         "Il task specificato non ha annotazioni che possano essere cancellate."
#define STRING_CMD_DENO_FOUND        "Annotazione '{1}' trovata e cancellata."
#define STRING_CMD_DENO_NOMATCH      "Nessuna annotazione da cancellare trovata per '{1}'."
#define STRING_CMD_DENO_NO           "Annotazione non rimossa."
#define STRING_CMD_DENO_1            "Annotazione rimossa da task {1}."
#define STRING_CMD_DENO_N            "Annotazioni rimosse da task {1}."

#define STRING_CMD_SYNC_SUCCESS0     "Sincronizzazione effettuata."
#define STRING_CMD_SYNC_SUCCESS1     "Sincronizzazione effettuata.  {1} modifiche caricate."
#define STRING_CMD_SYNC_SUCCESS2     "Sincronizzazione effettuata.  {1} modifiche scaricate."
#define STRING_CMD_SYNC_SUCCESS3     "Sincronizzazione effettuata.  {1} modifiche caricate, {2} modifiche scaricate."
#define STRING_CMD_SYNC_SUCCESS_NOP  "Sincronizzazione effettuata.  Nessuna modifica."
#define STRING_CMD_SYNC_FAIL_ACCOUNT "Sincronizzazione fallita.  Credenziali non corrette o Task Server non abilitato."
#define STRING_CMD_SYNC_FAIL_ERROR   "Sincronizzazione fallita.  Il Task Server ha ritornato l'errore: {1} {2}"
#define STRING_CMD_SYNC_FAIL_CONNECT "Sincronizzazione fallita.  Impossibile connettersi al Task Server."
#define STRING_CMD_SYNC_BAD_SERVER   "Sincronizzazione fallita.  Impostazione di configurazione '{1}' malformata"
#define STRING_CMD_SYNC_NO_TLS       "Taskwarrior was built without GnuTLS support.  Sync is not available."
#define STRING_CMD_DIAG_UNKNOWN      "<sconoscito>"
#define STRING_CMD_DIAG_COMPILER     "Compilatore"
#define STRING_CMD_DIAG_VERSION      "Versione"
#define STRING_CMD_DIAG_CAPS         "Caps"
#define STRING_CMD_DIAG_FEATURES     "Caratteristiche della Build"
#define STRING_CMD_DIAG_BUILT        "Generato"
#define STRING_CMD_DIAG_COMMIT       "Commit"
#define STRING_CMD_DIAG_UUID_SCAN    "Valutati {1} task per duplicazione UUID:"
#define STRING_CMD_DIAG_UUID_DUP     "Trovato duplicato {1}"
#define STRING_CMD_DIAG_UUID_NO_DUP  "Nessun duplicato trovato"

#define STRING_CMD_MODIFY_NO_DUE     "Impossibile specificare un task periodico senza una data di scadenza."
#define STRING_CMD_MODIFY_TASK       "Modifica del task {1} '{2}'."
#define STRING_CMD_MODIFY_TASK_R     "Modifica del task periodico {1} '{2}'."
#define STRING_CMD_MODIFY_1          "Task {1} modificato."
#define STRING_CMD_MODIFY_N          "Task {1} modificati."
#define STRING_CMD_MODIFY_NO         "Task non modificato."

#define STRING_CMD_TIMESHEET_STARTED "Avviati ({1} task)"
#define STRING_CMD_TIMESHEET_DONE    "Completati ({1} task)"
#define STRING_CMD_BURN_STARTED      "Avviato"          // Must be 7 or fewer characters
#define STRING_CMD_BURN_DONE         "Chiuso"             // Must be 7 or fewer characters
#define STRING_CMD_BURN_PENDING      "Da fare"          // Must be 7 or fewer characters

// Context
#define STRING_CONTEXT_CREATE_RC     "Un file di configurazione non può essere trovato in {1}\n\nSi desidera la creazione di un file di esempio {2}, in modo che taskwarrior possa procedere?"
#define STRING_CONTEXT_NEED_RC       "Impossibile procedere senza rc file."
#define STRING_CONTEXT_SHADOW_P      "Variabile di configurazione 'shadow.file' impostata a " "sovrascrittura dei task pendenti. Si prega di modificarla."
#define STRING_CONTEXT_SHADOW_C      "Variabile di configurazione 'shadow.file' impostata a " "sovrascrittura dei task completati. Si preda di modificarla."
#define STRING_CONTEXT_SHADOW_U      "Variabile di configurazione 'shadow.file' impostata a " "sovrascrittura del log di undo. Si prega di modificarla."
#define STRING_CONTEXT_SHADOW_B      "Variabile di configurazione 'shadow.file' impostata a " "sovrascrittura del file di backlog. Si prega di modificarla."
#define STRING_CONTEXT_SHADOW_UPDATE "[Shadow file '{1}' aggiornato.]"

// Date
#define STRING_DATE_BAD_WEEKSTART    "La variabile di configurazione 'weekstart' può solo contenere 'Domenica' o 'Lunedì'."

#define STRING_DATE_JANUARY_LONG     "gennaio"
#define STRING_DATE_FEBRUARY_LONG    "febbraio"
#define STRING_DATE_MARCH_LONG       "marzo"
#define STRING_DATE_APRIL_LONG       "aprile"
#define STRING_DATE_MAY_LONG         "maggio"
#define STRING_DATE_JUNE_LONG        "giugno"
#define STRING_DATE_JULY_LONG        "luglio"
#define STRING_DATE_AUGUST_LONG      "agosto"
#define STRING_DATE_SEPTEMBER_LONG   "settembre"
#define STRING_DATE_OCTOBER_LONG     "ottobre"
#define STRING_DATE_NOVEMBER_LONG    "novembre"
#define STRING_DATE_DECEMBER_LONG    "decembre"

#define STRING_DATE_JANUARY_SHORT    "gen"
#define STRING_DATE_FEBRUARY_SHORT   "feb"
#define STRING_DATE_MARCH_SHORT      "mar"
#define STRING_DATE_APRIL_SHORT      "apr"
#define STRING_DATE_MAY_SHORT        "mag"
#define STRING_DATE_JUNE_SHORT       "giu"
#define STRING_DATE_JULY_SHORT       "lug"
#define STRING_DATE_AUGUST_SHORT     "ago"
#define STRING_DATE_SEPTEMBER_SHORT  "set"
#define STRING_DATE_OCTOBER_SHORT    "ott"
#define STRING_DATE_NOVEMBER_SHORT   "nov"
#define STRING_DATE_DECEMBER_SHORT   "dic"

#define STRING_DATE_SUNDAY_LONG      "domenica"
#define STRING_DATE_MONDAY_LONG      "lunedì"
#define STRING_DATE_TUESDAY_LONG     "martedì"
#define STRING_DATE_WEDNESDAY_LONG   "mercoledì"
#define STRING_DATE_THURSDAY_LONG    "giovedì"
#define STRING_DATE_FRIDAY_LONG      "venerdì"
#define STRING_DATE_SATURDAY_LONG    "sabato"

#define STRING_DATE_SUNDAY_SHORT     "dom"
#define STRING_DATE_MONDAY_SHORT     "lun"
#define STRING_DATE_TUESDAY_SHORT    "mar"
#define STRING_DATE_WEDNESDAY_SHORT  "mer"
#define STRING_DATE_THURSDAY_SHORT   "gio"
#define STRING_DATE_FRIDAY_SHORT     "ven"
#define STRING_DATE_SATURDAY_SHORT   "sab"

// edit
#define STRING_EDIT_END              "Fine"

#define STRING_EDIT_END_MOD          "Data di fine modificata."
#define STRING_EDIT_END_DEL          "Data di fine rimossa."
#define STRING_EDIT_END_SET_ERR      "Impossibile impostare la data di chiusura del task pendente."
#define STRING_EDIT_UDA_MOD          "UDA {1} modificato."
#define STRING_EDIT_UDA_DEL          "UDA {1} cancellato."

// These four blocks can be replaced, but the number of lines must not change.
#define STRING_EDIT_HEADER_1         "Il comando 'task <id> edit' permette di modificare ogni aspetto di un task"
#define STRING_EDIT_HEADER_2         "utilizzando un editor di testo. Segue la rappresentazione dei dettagli di un task."
#define STRING_EDIT_HEADER_3         "Modificare quanto desiderato, e quando si salva e si esce dall'editor,"
#define STRING_EDIT_HEADER_4         "taskwarrior leggerà il file, determinando ciò che è cambiato, e applicherà"
#define STRING_EDIT_HEADER_5         "le modifiche. Se si esce dall'editor senza salvare o effettuare"
#define STRING_EDIT_HEADER_6         "modifiche, taskwarrior non farà alcunché."

#define STRING_EDIT_HEADER_7         "Le linee che iniziano con # rappresentano dati che non possono essere modificati, come l'ID."
#define STRING_EDIT_HEADER_8         "Se si è troppo creativi con le modifiche, taskwarrior aprirà"
#define STRING_EDIT_HEADER_9         "nuovamente l'editor per un secondo tentativo."

#define STRING_EDIT_HEADER_10        "Se ci si trova in un ciclo senza fine, continuando a editare lo stesso file,"
#define STRING_EDIT_HEADER_11        "si chiuda semplicemente l'editor senza effettuare modifiche. Taskwarrior"
#define STRING_EDIT_HEADER_12        "se ne accorgerà e terminerà l'editing."

// Maintain the same spacing.
#define STRING_EDIT_TABLE_HEADER_1   "Nome               Dati modificabili"
#define STRING_EDIT_TABLE_HEADER_2   "-----------------  ----------------------------------------------------"

// Feedback
#define STRING_FEEDBACK_TASKS_SINGLE "(1 task)"
#define STRING_FEEDBACK_TASKS_PLURAL "({1} task)"
#define STRING_FEEDBACK_DELETED      "{1} sarà cancellato."
#define STRING_FEEDBACK_DEP_SET      "Le dipendenze saranno impostate a '{1}'."
#define STRING_FEEDBACK_DEP_MOD      "Le dipendenze saranno modificate da '{1}' in '{2}'."
#define STRING_FEEDBACK_DEP_DEL      "Dipendenze '{1}' cancellate."
#define STRING_FEEDBACK_DEP_WAS_SET  "Dipendenze impostate a '{1}'."
#define STRING_FEEDBACK_DEP_WAS_MOD  "Dipendenze cambiate da '{1}' a '{2}'."
#define STRING_FEEDBACK_ATT_SET      "{1} sarà impostata a '{2}'."
#define STRING_FEEDBACK_ATT_MOD      "{1} sarà modificata da '{2}' a '{3}'."
#define STRING_FEEDBACK_ATT_DEL      "{1} cancellato."
#define STRING_FEEDBACK_ATT_DEL_DUR  "{1} cancellato (durata: {2})."
#define STRING_FEEDBACK_ATT_WAS_SET  "{1} impostata a '{2}'."
#define STRING_FEEDBACK_ATT_WAS_MOD  "{1} modificata da '{2}' a '{3}'."
#define STRING_FEEDBACK_ANN_ADD      "Annotazione di '{1}' aggiunta."
#define STRING_FEEDBACK_ANN_DEL      "Annotazione '{1}' cancellata."
#define STRING_FEEDBACK_ANN_WAS_MOD  "Annotazione modificata in '{1}'."
#define STRING_FEEDBACK_UNBLOCKED    "Sbloccato {1} '{2}'."
#define STRING_FEEDBACK_EXPIRED      "Il task {1} '{2}' è scaduto ed è stato eliminato"

// 'show' command
#define STRING_CMD_SHOW              "Mostra i sottoinsiemi di variabili di configurazione"
#define STRING_CMD_SHOW_UNREC        "Il file .taskrc contiene variabili non riconosciute:"
#define STRING_CMD_SHOW_DIFFER       "Alcune variabili in .taskrc hanno valore diverso dal default."
#define STRING_CMD_SHOW_DIFFER_COLOR "Evidenziate sopra in {1}."

// Task
#define STRING_TASK_VALID_PRIORITY   "I valori di priorità possono essere 'H', 'M' o 'L', non '{1}'."

// Taskmod
#define STRING_TASKMOD_TIME          "tempo "
#define STRING_TASKMOD_OLD           "vecchio "
#define STRING_TASKMOD_NEW           "nuovo "

// TDB2
#define STRING_TDB2_REMOTE_CHANGE    "Trovata modifica remota a        {1}  \"{2}\""
#define STRING_TDB2_LOCAL_CHANGE     "Mantenute le modifiche locali a    {1}  \"{2}\""
#define STRING_TDB2_MISSING          "Mancante                       {1}  \"{2}\""
#define STRING_TDB2_MERGING          "Unione di nuovi task remoti       {1}  \"{2}\""
#define STRING_TDB2_LAST_MOD         "L'ultima modifica è stat effettuata {1}"
#define STRING_TDB2_DIFF_PREV        "--- stato precedente"             // Same length
#define STRING_TDB2_DIFF_PREV_DESC   "L'undo ripristinerà questo stato"   //   ||
#define STRING_TDB2_DIFF_CURR        "+++ stato corrente "             // Same length
#define STRING_TDB2_DIFF_CURR_DESC   "Modifiche effettuate {1}"
#define STRING_TDB2_MISSING_UUID     "Impossibile individuare l'UUID nel task da ripristinare."
#define STRING_TDB2_MISSING_TASK     "Task con UUID {1} non trovato nei dati."

// shell
#define STRING_SHELL_USAGE \
  "Uso: tasksh [<commands-file>]      Esegue i comandi all'interno di <commands-file> se fornito,\n" \
  "                                   oppure avvia la shell interattiva.\n" \
  "       tasksh --version            Stampa la versione di task.\n" \
  "       tasksh --help               Stampa questo aiuto.\n"

#define STRING_SHELL_NO_FILE         "Il file di input non esiste.\n"

#endif

