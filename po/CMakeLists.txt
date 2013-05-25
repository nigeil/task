set (PO_FILES es.po fr.po it.po)

if (HAVE_GETTEXT)
  GETTEXT_CREATE_TRANSLATIONS (task.pot ALL ${PO_FILES})
endif (HAVE_GETTEXT)
