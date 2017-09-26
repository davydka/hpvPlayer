(pidof -c /home/davydka/processing-2.2.1/java/bin/java > /dev/null || /home/davydka/processing-2.2.1/processing-java --run --sketch=/home/davydka/of/apps/Sites/hpvPlayer/utils/src/screenCapture --output=/home/davydka/of/apps/Sites/hpvPlayer/utils/src/screenCapture/output --force &> /home/davydka/of/apps/Sites/hpvPlayer/output-processing.log &)

(pidof -c hpvPlayer > /dev/null || /home/davydka/of/apps/Sites/hpvPlayer/bin/hpvPlayer &> /home/davydka/of/apps/Sites/hpvPlayer/output-hpv.log &)

sleep 4
wmctrl -r "hpvPlayer" -b add,above;

`ffmpeg -i movie.mov -r 60 output/$filename%05d.bmp`


sendmidi dev "mio MIDI 1" ch 4 on 40 120; sleep 1; sendmidi dev "mio MIDI 1" ch 4 off 40 0;
sendmidi dev "IAC Driver Bus 1" ch 4 on 1 120; sleep 1; sendmidi dev "IAC Driver Bus 1" ch 4 off 1 0;

