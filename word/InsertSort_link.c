//链表的直接插入排序

void insert_sort(List head)//插入排序，传入头节点
{
	List p = head->next, q, temp;
	if (p)
	{
		for (p = p->next; p; p = p->next)//从第二个数开始，第一个数默认有序
		{
			temp = p;
			for (q = p->per; q != head; q = q->per)//q从p的前一节点开始往前遍历
			{
				if (q->data <= temp->data)//升序
				{
					break;
				}
			}
			if (q->next != p)//如果q有前移，插入temp
			{
				if (temp->next)//temp不为最后一个节点
				{
					//temp前后互指，分离temp
					temp->per->next = temp->next;
					temp->next->per = temp->per;

					//插入temp
					temp->next = q->next;
					q->next->per = temp;
					temp->per = q;
					q->next = temp;
				}
				else //temp是最后一个节点
				{
					temp->per->next = NULL;//temp前一节点的next指为NULL
					
					//插入temp
					temp->next = q->next;
					q->next->per = temp;
					temp->per = q;
					q->next = temp;
				}
			}
		}
		printf("已排序\n");
	}
	else
	{
		printf("链表为空\n");
	}
}
————————————————
版权声明：本文为CSDN博主「_是真的」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/m0_46254928/article/details/109719119