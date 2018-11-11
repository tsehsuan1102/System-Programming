

//前置處理
先建立一個Item的struct(id, amount, price)，

//file
fd為用來打開Item_list的file descripter.
rset是記錄有連線著的connection


//I/O multiplexing
每次把maxfd以內的conn_fd都丟進rset
然後用select(ty設NULL)
接著檢查每一個request，如果有變動那就會留在rset裡面，
我們就可以對他做操作。

//read server
nowid表示當前client所輸入的id
Item now是該id對應到的Item
然後確認RDLCK是否可以上鎖，
鎖成功就用pread讀該段的值
然後把對應的資料輸出出來
別忘了最後要unlock那段

//write server
第一階段跟read server差不多
只是lock要改成WRLCK
喔對了 由於write的時候只能有一個client
所以不只要看lock，
也要看看同一個process內有沒有其他request也在用那個item
所以用一個迴圈檢查所以request的item_id
都不同才可以繼續，否則視為lock狀態

第二階段
一樣透過handle_read讀輸入
然後依據題意判斷要+/-數量，改price
最後pwrite寫回檔案內
在unlock就結束了






