Execution跟題目要求的用法一樣
./bidding_system host_num player_num

我分享一個小故事，就是我某天在寫這個作業的時後，因為很怕把工作站弄爛，所以每次我都會ps檢查看看有沒有process沒停下來。但某一次一執行之後，不知道為甚麼沒有停下來，生了一堆zombie然後不斷地fork。
我就被linux1禁止fork了，但bash還是有，不過只能用build-in的function。
接著我到linux2繼續做，然後又發生一次，這次我直接被踢掉，然後連線被拒，連不回linux2了，最後這份作業是在linux3上面寫完的QQ

# bidding_system
先從argv[1], argv[2]讀入hostnum和playernum，
然後fork出hostnum個child去跑host，對於每一個host都建立兩個pipe，其中pipe_in是child寫給parent，而pipe_out是parent寫給child
然後預先算好C(N,4)的所有分配，存在array裡面。
先給每一台host一項比賽(如果host太多會在發完所有比賽break掉)，然後開select去聽他們的return，如果有聽到return，就會去更新一下分數，然後派給他下一場比賽，最後在依照規則去計算排名就結束了。

# host
一開始先創建五個FIFO，一個讀回傳，4個寫給player，然後fork出4個player，跑10個回合，對每個player都告訴她當前所有player的所有money，然後去收他們的出價結果，更新持有物品數量，十個回合後，回傳排名給bidding_system。

# player
也跑一個迴圈，每回合等host給資訊就開始算，讀到host的訊息後，依照規則，如果這回合輪到自己出價，就出所有的錢，否則回傳0





