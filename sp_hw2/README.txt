Execution���D�حn�D���Ϊk�@��
./bidding_system host_num player_num

�ڤ��ɤ@�Ӥp�G�ơA�N�O�ڬY�Ѧb�g�o�ӧ@�~���ɫ�A�]���ܩȧ�u�@������A�ҥH�C���ڳ��|ps�ˬd�ݬݦ��S��process�S���U�ӡC���Y�@���@���椧��A�����D���ƻ�S�����U�ӡA�ͤF�@��zombie�M�ᤣ�_�afork�C
�ڴN�Qlinux1�T��fork�F�A��bash�٬O���A���L�u���build-in��function�C
���ۧڨ�linux2�~�򰵡A�M��S�o�ͤ@���A�o���ڪ����Q�𱼡A�M��s�u�Q�ڡA�s���^linux2�F�A�̫�o���@�~�O�blinux3�W���g����QQ

# bidding_system
���qargv[1], argv[2]Ū�Jhostnum�Mplayernum�A
�M��fork�Xhostnum��child�h�]host�A���C�@��host���إߨ��pipe�A�䤤pipe_in�Ochild�g��parent�A��pipe_out�Oparent�g��child
�M��w����nC(N,4)���Ҧ����t�A�s�barray�̭��C
�����C�@�xhost�@������(�p�Ghost�Ӧh�|�b�o���Ҧ�����break��)�A�M��}select�hť�L�̪�return�A�p�G��ť��return�A�N�|�h��s�@�U���ơA�M�ᬣ���L�U�@�����ɡA�̫�b�̷ӳW�h�h�p��ƦW�N�����F�C

# host
�@�}�l���Ыؤ���FIFO�A�@��Ū�^�ǡA4�Ӽg��player�A�M��fork�X4��player�A�]10�Ӧ^�X�A��C��player���i�D�o��e�Ҧ�player���Ҧ�money�A�M��h���L�̪��X�����G�A��s�������~�ƶq�A�Q�Ӧ^�X��A�^�ǱƦW��bidding_system�C

# player
�]�]�@�Ӱj��A�C�^�X��host����T�N�}�l��AŪ��host���T����A�̷ӳW�h�A�p�G�o�^�X����ۤv�X���A�N�X�Ҧ������A�_�h�^��0





