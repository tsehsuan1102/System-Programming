

ps -aux | grep 'tsehsuan' | cut -d ' ' -f 2 > for_kill

exec < for_kill

while read line
    do
        kill -9 $line
    done

rm -rf for_kill

