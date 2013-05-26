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
#define STRING_COLUMN_LABEL_TASKS    "Tasks"
#define STRING_COLUMN_LABEL_DESC     "Description"
#define STRING_COLUMN_LABEL_DUE      "Due"
#define STRING_COLUMN_LABEL_ENTERED  "Entered"
#define STRING_COLUMN_LABEL_COUNT    "Count"
#define STRING_COLUMN_LABEL_PROJECT  "Project"
#define STRING_COLUMN_LABEL_STAT     "St"
#define STRING_COLUMN_LABEL_STAT_PE  "Pending"
#define STRING_COLUMN_LABEL_STAT_CO  "Completed"
#define STRING_COLUMN_LABEL_STAT_DE  "Deleted"
#define STRING_COLUMN_LABEL_STAT_WA  "Waiting"
#define STRING_COLUMN_LABEL_STAT_RE  "Recurring"
#define STRING_COLUMN_LABEL_STAT_P   "P"
#define STRING_COLUMN_LABEL_STAT_C   "C"
#define STRING_COLUMN_LABEL_STAT_D   "D"
#define STRING_COLUMN_LABEL_STAT_W   "W"
#define STRING_COLUMN_LABEL_STAT_R   "R"
#define STRING_COLUMN_LABEL_TAGS     "Tags"
#define STRING_COLUMN_LABEL_TAG      "Tag"
#define STRING_COLUMN_LABEL_UUID     "UUID"
#define STRING_COLUMN_LABEL_URGENCY  "Urgency"
#define STRING_COLUMN_LABEL_VALUE    "Value"
#define STRING_COLUMN_LABEL_DATE     "Date"
#define STRING_COLUMN_LABEL_SCHED    "Scheduled"
#define STRING_COLUMN_LABEL_UDA      "Name"
#define STRING_COLUMN_LABEL_TYPE     "Type"
#define STRING_COLUMN_LABEL_LABEL    "Label"
#define STRING_COLUMN_LABEL_DEFAULT  "Default"
#define STRING_COLUMN_LABEL_VALUES   "Allowed Values"
#define STRING_COLUMN_LABEL_UDACOUNT "Usage Count"
#define STRING_COLUMN_LABEL_ORPHAN   "Orphan UDA"

// commands/Cmd*
#define STRING_CMD_UDAS_USAGE        "Shows all the defined UDA details"
#define STRING_CMD_UDAS_COMPL_USAGE  "Shows the defined UDAs for completion purposes"
#define STRING_CMD_INFO_BLOCKED      "This task blocked by"
#define STRING_CMD_INFO_BLOCKING     "This task is blocking"
#define STRING_CMD_INFO_MODIFICATION "Modification"
#define STRING_CMD_INFO_MODIFIED     "Last modified"
#define STRING_CMD_STATS_CATEGORY    "Category"
#define STRING_CMD_STATS_DATA        "Data"
#define STRING_CMD_STATS_TOTAL       "Total"
#define STRING_CMD_STATS_ANNOTATIONS "Annotations"
#define STRING_CMD_STATS_UNIQUE_TAGS "Unique tags"
#define STRING_CMD_STATS_PROJECTS    "Projects"
#define STRING_CMD_STATS_DATA_SIZE   "Data size"
#define STRING_CMD_STATS_UNDO_TXNS   "Undo transactions"
#define STRING_CMD_STATS_BACKLOG     "Sync backlog transactions"
#define STRING_CMD_STATS_TAGGED      "Tasks tagged"
#define STRING_CMD_STATS_OLDEST      "Oldest task"
#define STRING_CMD_STATS_NEWEST      "Newest task"
#define STRING_CMD_STATS_USED_FOR    "Task used for"
#define STRING_CMD_STATS_ADD_EVERY   "Task added every"
#define STRING_CMD_STATS_COMP_EVERY  "Task completed every"
#define STRING_CMD_STATS_DEL_EVERY   "Task deleted every"
#define STRING_CMD_STATS_AVG_PEND    "Average time pending"
#define STRING_CMD_STATS_DESC_LEN    "Average desc length"
#define STRING_CMD_STATS_CHARS       "{1} characters"
#define STRING_CMD_STATS_LAST_SYNC   "Last server synchronization"
#define STRING_CMD_STATS_BLOCKED     "Blocked tasks"
#define STRING_CMD_STATS_BLOCKING    "Blocking tasks"
#define STRING_CMD_TAGS_SINGLE       "1 tag"
#define STRING_CMD_TAGS_PLURAL       "{1} tags"
#define STRING_CMD_HISTORY_YEAR      "Year"
#define STRING_CMD_HISTORY_MONTH     "Month"
#define STRING_CMD_HISTORY_ADDED     "Added"
#define STRING_CMD_HISTORY_COMP      "Completed"
#define STRING_CMD_HISTORY_DEL       "Deleted"
#define STRING_CMD_HISTORY_NET       "Net"
#define STRING_CMD_HISTORY_LEGEND    "Legend: {1}, {2}, {3}"
#define STRING_CMD_GHISTORY_YEAR     "Year"
#define STRING_CMD_GHISTORY_MONTH    "Month"
#define STRING_CMD_GHISTORY_NUMBER   "Number Added/Completed/Deleted"

#define STRING_CMD_DONE_NO           "Task not completed."
#define STRING_CMD_DONE_NOTPEND      "Task {1} '{2}' is neither pending nor waiting."
#define STRING_CMD_DONE_1            "Completed {1} task."
#define STRING_CMD_DONE_N            "Completed {1} tasks."

#define STRING_CMD_PROJECTS_PRI_N    "Pri:None"
#define STRING_CMD_PROJECTS_PRI_H    "Pri:H"
#define STRING_CMD_PROJECTS_PRI_M    "Pri:M"
#define STRING_CMD_PROJECTS_PRI_L    "Pri:L"
#define STRING_CMD_PROJECTS_SUMMARY  "{1} project"
#define STRING_CMD_PROJECTS_SUMMARY2 "{1} projects"
#define STRING_CMD_PROJECTS_TASK     "({1} task)"
#define STRING_CMD_PROJECTS_TASKS    "({1} tasks)"
#define STRING_CMD_SUMMARY_PROJECT   "Project"
#define STRING_CMD_SUMMARY_REMAINING "Remaining"
#define STRING_CMD_SUMMARY_AVG_AGE   "Avg age"
#define STRING_CMD_SUMMARY_COMPLETE  "Complete"

#define STRING_CMD_UDAS_NO           "No UDAs defined."
#define STRING_CMD_UDAS_SUMMARY      "{1} UDA defined"
#define STRING_CMD_UDAS_SUMMARY2     "{1} UDAs defined"
#define STRING_CMD_UDAS_ORPHAN       "{1} Orphan UDA"
#define STRING_CMD_UDAS_ORPHANS      "{1} Orphan UDAs"

#define STRING_CMD_DUPLICATE_1       "Duplicated {1} task."
#define STRING_CMD_DUPLICATE_N       "Duplicated {1} tasks."

#define STRING_CMD_PREPEND_1         "Prepended {1} task."
#define STRING_CMD_PREPEND_N         "Prepended {1} tasks."

#define STRING_CMD_DENO_NONE         "The specified task has no annotations that can be deleted."
#define STRING_CMD_DENO_FOUND        "Found annotation '{1}' and deleted it."
#define STRING_CMD_DENO_NOMATCH      "Did not find any matching annotation to be deleted for '{1}'."
#define STRING_CMD_DENO_NO           "Task not denotated."
#define STRING_CMD_DENO_1            "Denotated {1} task."
#define STRING_CMD_DENO_N            "Denotated {1} tasks."

#define STRING_CMD_SYNC_SUCCESS0     "Sync successful."
#define STRING_CMD_SYNC_SUCCESS1     "Sync successful.  {1} changes uploaded."
#define STRING_CMD_SYNC_SUCCESS2     "Sync successful.  {1} changes downloaded."
#define STRING_CMD_SYNC_SUCCESS3     "Sync successful.  {1} changes uploaded, {2} changes downloaded."
#define STRING_CMD_SYNC_SUCCESS_NOP  "Sync successful.  No changes."
#define STRING_CMD_SYNC_FAIL_ACCOUNT "Sync failed.  Either your credentials are incorrect, or your Task Server account is not enabled."
#define STRING_CMD_SYNC_FAIL_ERROR   "Sync failed.  The Task Server returned error: {1} {2}"
#define STRING_CMD_SYNC_FAIL_CONNECT "Sync failed.  Could not connect to the Task Server."
#define STRING_CMD_SYNC_BAD_SERVER   "Sync failed.  Malformed configuration setting '{1}'"
#define STRING_CMD_SYNC_NO_TLS       "Taskwarrior was built without GnuTLS support.  Sync is not available."
#define STRING_CMD_DIAG_UNKNOWN      "<unknown>"
#define STRING_CMD_DIAG_COMPILER     "Compiler"
#define STRING_CMD_DIAG_VERSION      "Version"
#define STRING_CMD_DIAG_CAPS         "Caps"
#define STRING_CMD_DIAG_FEATURES     "Build Features"
#define STRING_CMD_DIAG_BUILT        "Built"
#define STRING_CMD_DIAG_COMMIT       "Commit"
#define STRING_CMD_DIAG_UUID_SCAN    "Scanned {1} tasks for duplicate UUIDs:"
#define STRING_CMD_DIAG_UUID_DUP     "Found duplicate {1}"
#define STRING_CMD_DIAG_UUID_NO_DUP  "No duplicates found"

#define STRING_CMD_MODIFY_NO_DUE     "You cannot specify a recurring task without a due date."
#define STRING_CMD_MODIFY_TASK       "Modifying task {1} '{2}'."
#define STRING_CMD_MODIFY_TASK_R     "Modifying recurring task {1} '{2}'."
#define STRING_CMD_MODIFY_1          "Modified {1} task."
#define STRING_CMD_MODIFY_N          "Modified {1} tasks."
#define STRING_CMD_MODIFY_NO         "Task not modified."

#define STRING_CMD_TIMESHEET_STARTED "Started ({1} tasks)"
#define STRING_CMD_TIMESHEET_DONE    "Completed ({1} tasks)"
#define STRING_CMD_BURN_STARTED      "Started"          // Must be 7 or fewer characters
#define STRING_CMD_BURN_DONE         "Done"             // Must be 7 or fewer characters
#define STRING_CMD_BURN_PENDING      "Pending"          // Must be 7 or fewer characters

// Context
#define STRING_CONTEXT_CREATE_RC     "A configuration file could not be found in {1}\n\nWould you like a sample {2} created, so taskwarrior can proceed?"
#define STRING_CONTEXT_NEED_RC       "Cannot proceed without rc file."
#define STRING_CONTEXT_SHADOW_P      "Configuration variable 'shadow.file' is set to " "overwrite your pending tasks.  Please change it."
#define STRING_CONTEXT_SHADOW_C      "Configuration variable 'shadow.file' is set to " "overwrite your completed tasks.  Please change it."
#define STRING_CONTEXT_SHADOW_U      "Configuration variable 'shadow.file' is set to " "overwrite your undo log.  Please change it."
#define STRING_CONTEXT_SHADOW_B      "Configuration variable 'shadow.file' is set to " "overwrite your backlog file.  Please change it."
#define STRING_CONTEXT_SHADOW_UPDATE "[Shadow file '{1}' updated.]"

// Date
#define STRING_DATE_BAD_WEEKSTART    "The 'weekstart' configuration variable may only contain 'Sunday' or 'Monday'."

#define STRING_DATE_JANUARY_LONG     "january"
#define STRING_DATE_FEBRUARY_LONG    "february"
#define STRING_DATE_MARCH_LONG       "march"
#define STRING_DATE_APRIL_LONG       "april"
#define STRING_DATE_MAY_LONG         "may"
#define STRING_DATE_JUNE_LONG        "june"
#define STRING_DATE_JULY_LONG        "july"
#define STRING_DATE_AUGUST_LONG      "august"
#define STRING_DATE_SEPTEMBER_LONG   "september"
#define STRING_DATE_OCTOBER_LONG     "october"
#define STRING_DATE_NOVEMBER_LONG    "november"
#define STRING_DATE_DECEMBER_LONG    "december"

#define STRING_DATE_JANUARY_SHORT    "jan"
#define STRING_DATE_FEBRUARY_SHORT   "feb"
#define STRING_DATE_MARCH_SHORT      "mar"
#define STRING_DATE_APRIL_SHORT      "apr"
#define STRING_DATE_MAY_SHORT        "may"
#define STRING_DATE_JUNE_SHORT       "jun"
#define STRING_DATE_JULY_SHORT       "jul"
#define STRING_DATE_AUGUST_SHORT     "aug"
#define STRING_DATE_SEPTEMBER_SHORT  "sep"
#define STRING_DATE_OCTOBER_SHORT    "oct"
#define STRING_DATE_NOVEMBER_SHORT   "nov"
#define STRING_DATE_DECEMBER_SHORT   "dec"

#define STRING_DATE_SUNDAY_LONG      "sunday"
#define STRING_DATE_MONDAY_LONG      "monday"
#define STRING_DATE_TUESDAY_LONG     "tuesday"
#define STRING_DATE_WEDNESDAY_LONG   "wednesday"
#define STRING_DATE_THURSDAY_LONG    "thursday"
#define STRING_DATE_FRIDAY_LONG      "friday"
#define STRING_DATE_SATURDAY_LONG    "saturday"

#define STRING_DATE_SUNDAY_SHORT     "sun"
#define STRING_DATE_MONDAY_SHORT     "mon"
#define STRING_DATE_TUESDAY_SHORT    "tue"
#define STRING_DATE_WEDNESDAY_SHORT  "wed"
#define STRING_DATE_THURSDAY_SHORT   "thu"
#define STRING_DATE_FRIDAY_SHORT     "fri"
#define STRING_DATE_SATURDAY_SHORT   "sat"

// edit
#define STRING_EDIT_END              "End"

#define STRING_EDIT_END_MOD          "End date modified."
#define STRING_EDIT_END_DEL          "End date removed."
#define STRING_EDIT_END_SET_ERR      "Cannot set a done date on a pending task."
#define STRING_EDIT_UDA_MOD          "UDA {1} modified."
#define STRING_EDIT_UDA_DEL          "UDA {1} deleted."

// These four blocks can be replaced, but the number of lines must not change.
#define STRING_EDIT_HEADER_1         "The 'task <id> edit' command allows you to modify all aspects of a task"
#define STRING_EDIT_HEADER_2         "using a text editor.  Below is a representation of all the task details."
#define STRING_EDIT_HEADER_3         "Modify what you wish, and when you save and quit your editor,"
#define STRING_EDIT_HEADER_4         "taskwarrior will read this file, determine what changed, and apply"
#define STRING_EDIT_HEADER_5         "those changes.  If you exit your editor without saving or making"
#define STRING_EDIT_HEADER_6         "modifications, taskwarrior will do nothing."

#define STRING_EDIT_HEADER_7         "Lines that begin with # represent data you cannot change, like ID."
#define STRING_EDIT_HEADER_8         "If you get too creative with your editing, taskwarrior will send you"
#define STRING_EDIT_HEADER_9         "back to the editor to try again."

#define STRING_EDIT_HEADER_10        "Should you find yourself in an endless loop, re-editing the same file,"
#define STRING_EDIT_HEADER_11        "just quit the editor without making any changes.  Taskwarrior will"
#define STRING_EDIT_HEADER_12        "notice this and stop the editing."

// Maintain the same spacing.
#define STRING_EDIT_TABLE_HEADER_1   "Name               Editable details"
#define STRING_EDIT_TABLE_HEADER_2   "-----------------  ----------------------------------------------------"

// Feedback
#define STRING_FEEDBACK_TASKS_SINGLE "(1 task)"
#define STRING_FEEDBACK_TASKS_PLURAL "({1} tasks)"
#define STRING_FEEDBACK_DELETED      "{1} will be deleted."
#define STRING_FEEDBACK_DEP_SET      "Dependencies will be set to '{1}'."
#define STRING_FEEDBACK_DEP_MOD      "Dependencies will be changed from '{1}' to '{2}'."
#define STRING_FEEDBACK_DEP_DEL      "Dependencies '{1}' deleted."
#define STRING_FEEDBACK_DEP_WAS_SET  "Dependencies set to '{1}'."
#define STRING_FEEDBACK_DEP_WAS_MOD  "Dependencies changed from '{1}' to '{2}'."
#define STRING_FEEDBACK_ATT_SET      "{1} will be set to '{2}'."
#define STRING_FEEDBACK_ATT_MOD      "{1} will be changed from '{2}' to '{3}'."
#define STRING_FEEDBACK_ATT_DEL      "{1} deleted."
#define STRING_FEEDBACK_ATT_DEL_DUR  "{1} deleted (duration: {2})."
#define STRING_FEEDBACK_ATT_WAS_SET  "{1} set to '{2}'."
#define STRING_FEEDBACK_ATT_WAS_MOD  "{1} changed from '{2}' to '{3}'."
#define STRING_FEEDBACK_ANN_ADD      "Annotation of '{1}' added."
#define STRING_FEEDBACK_ANN_DEL      "Annotation '{1}' deleted."
#define STRING_FEEDBACK_ANN_WAS_MOD  "Annotation changed to '{1}'."
#define STRING_FEEDBACK_UNBLOCKED    "Unblocked {1} '{2}'."
#define STRING_FEEDBACK_EXPIRED      "Task {1} '{2}' expired and was deleted."

// 'show' command
#define STRING_CMD_SHOW              "Shows all configuration variables or subset"
#define STRING_CMD_SHOW_UNREC        "Your .taskrc file contains these unrecognized variables:"
#define STRING_CMD_SHOW_DIFFER       "Some of your .taskrc variables differ from the default values."
#define STRING_CMD_SHOW_DIFFER_COLOR "These are highlighted in {1} above."

// Task
#define STRING_TASK_VALID_PRIORITY   "Priority values may be 'H', 'M' or 'L', not '{1}'."

// Taskmod
#define STRING_TASKMOD_TIME          "time "
#define STRING_TASKMOD_OLD           "old "
#define STRING_TASKMOD_NEW           "new "

// TDB2
#define STRING_TDB2_REMOTE_CHANGE    "Found remote change to        {1}  \"{2}\""
#define STRING_TDB2_LOCAL_CHANGE     "Retaining local changes to    {1}  \"{2}\""
#define STRING_TDB2_MISSING          "Missing                       {1}  \"{2}\""
#define STRING_TDB2_MERGING          "Merging new remote task       {1}  \"{2}\""
#define STRING_TDB2_LAST_MOD         "The last modification was made {1}"
#define STRING_TDB2_DIFF_PREV        "--- previous state"             // Same length
#define STRING_TDB2_DIFF_PREV_DESC   "Undo will restore this state"   //   ||
#define STRING_TDB2_DIFF_CURR        "+++ current state "             // Same length
#define STRING_TDB2_DIFF_CURR_DESC   "Change made {1}"
#define STRING_TDB2_MISSING_UUID     "Cannot locate UUID in task to undo."
#define STRING_TDB2_MISSING_TASK     "Task with UUID {1} not found in data."

// shell
#define STRING_SHELL_USAGE \
  "Usage: tasksh [<commands-file>]    Execute task commands inside <commands-file> if given,\n" \
  "                                   or otherwise, start interactive task shell.\n" \
  "       tasksh --version            Print task version.\n" \
  "       tasksh --help               Print this help.\n"

#define STRING_SHELL_NO_FILE         "Input file does not exist.\n"

#endif

