Execution:

����make�Ӳ��ͤT�Ӱ�����(bidding_system, bidding_system_EDF, customer)

[bidding_system]
./bidding_system
��pipe�M��fork�@��child
��stdout�ɨ�pipe
�M��hexec customer

parent��signal��SIGUSR1, SIGUSR2���O�@member �M customer��Ө禡��@handler�A�M��bpipeŪ��ordinary�ɥh�I�sordinary()�C
�禡������@nanosleep�A�M��Χ��^�ǵ�cutomer���signal�C

[customer]
��Ū�itest_data�A�M��nanosleep�������ӶǰT�����ɭ԰_�ӶǡA�A�ӹ�member�p�@��alarm(�]���u��member���i��timeout)�A�p�Gtimeout�N�n�gterminate�M������

[bidding_system_EDF]
��read pipe�令nonblocking�A�M��bpipe�_�����e�@�����ƶ]�j��A�̭��C���P�_�{�b�n���֡A�p�G��signal�i�ӴN�h��s��deadline�A�M�᭫�i�j��A���s�M�w�n���֡C










