xgettext \
    -k_ \
    -ksgettext \
    -c++ \
    -cTRANSLATORS \
    -j \
    --package-name taskwarrior \
    --package-version 2.3.0 \
    --copyright-holder '2006-2013, Paul Beckingham, Federico Hernandez.' \
    --msgid-bugs-address 'Taskwarrior team <support@taskwarrior.org>' \
    -o task.pot \
    $(find .. -name '*.cpp' -o -name '*.h')
