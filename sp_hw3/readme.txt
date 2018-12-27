Execution:

先用make來產生三個執行檔(bidding_system, bidding_system_EDF, customer)

[bidding_system]
./bidding_system
建pipe然後fork一個child
把stdout導到pipe
然後去exec customer

parent用signal對SIGUSR1, SIGUSR2分別作member 和 customer兩個函式當作handler，然後在pipe讀到ordinary時去呼叫ordinary()。
函式內部實作nanosleep，然後睡完回傳給cutomer停止的signal。

[customer]
先讀進test_data，然後nanosleep等到應該傳訊號的時候起來傳，再來對member計一個alarm(因為只有member有可能timeout)，如果timeout就要寫terminate然後關掉

[bidding_system_EDF]
把read pipe改成nonblocking，然後在pipe斷掉之前一直重複跑迴圈，裡面每次判斷現在要做誰，如果有signal進來就去算新的deadline，然後重進迴圈，重新決定要做誰。










