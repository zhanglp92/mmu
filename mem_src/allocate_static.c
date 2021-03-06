/*************************************************************************
	> File Name: allocate_equal.c
	> Author: zhanglp
	> Mail: 820221185@qq.com 
	> Created Time: 2013年11月19日 星期二 19时40分28秒
    > describe : allocate memory equal
 ************************************************************************/

#include "allocate_static.h"


/**
* @brief recv_msg 
*
* @param qid
* @param msgbuffer
* @param len
* @param f
*
* @describe : recv message from message's
*/
void recv_msg (int qid, struct mymsgbuf *msgbuffer, size_t len, int f)
{
    if (msgrcv (qid, msgbuffer, len, msgbuffer->msgtype, f) < 0) {
        perror ("msgsend o");
        exit (EXIT_FAILURE);
    }
}

/**
* @brief send_msg 
*
* @param qid
* @param msgbuffer
* @param len
* @param f
*
* @describe : send message to message queue
*/
void send_msg (int qid, struct mymsgbuf *msgbuffer, size_t len, int f)
{
    if (msgsnd (qid, msgbuffer, len, f) < 0) {
        perror ("msgsend o");
        exit (EXIT_FAILURE);
    }
}

/**
* @brief deal_msg_from_ps 
*
* @param qid
* @param msg
*
* @describe : deal message from process
*/
void deal_msg_from_ps (int qid, struct mymsgbuf msg)
{
    struct mymsgbuf     msgbuffer;

    switch (msg.msg) {

        case MSG_APPLY_MEM : {

            if ((msgbuffer.pid = allocate_mem (ps_size ())) >= 0) {
                msgbuffer.msgtype = MSG_TO_PS;
            }else {
                my_perror ("allocate_mem");
            }
            send_msg (qid, &msgbuffer, sizeof (struct mymsgbuf)-4, 0);
            print_mem (PRINT_NOT_MEM);
        }break;

        case MSG_PS_END : {
            destroy_mem (msg.pid);
        }break;
    }

    return ;
}


int main (void)
{
    _hit = _count = 1;

    int     qid;
    int     msglen;
    key_t   msgkey;

    if ((msgkey = ftok (PATH_NAME, PROJ_ID)) < 0) {
        perror ("ftok");
        exit (EXIT_FAILURE);
    }
    if ((qid = msgget (msgkey, IPC_CREAT | 0660)) < 0) {
        perror ("msgget");
        exit (EXIT_FAILURE);
    }

    init_block ();
    print_mem (PRINT_MEM);

    struct mymsgbuf     msgbuffer;
    while (1) {

        msgbuffer.msgtype = MSG_FROM_PS;
        recv_msg (qid, &msgbuffer, sizeof (struct mymsgbuf)-4, 0);
        deal_msg_from_ps (qid, msgbuffer);
    }

    return EXIT_SUCCESS;
}
