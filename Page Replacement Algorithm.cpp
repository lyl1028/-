#include <iostream>
#include <vector>

using namespace std;

int n, m, cur_time = 0, hitcount = 0;
int v[100];

struct page
{
    int pn;
    int pfn;
    int time;
} pages[10];

struct frame
{
    int pn;
    int pfn;
    frame* next = NULL;
};
vector<frame> frames;

frame* free_head = NULL, * busy_head = NULL, * busy_tail = NULL;

void init()
{
    for (int i = 0; i < 10; i++)
    {
        pages[i].pn = i;
        pages[i].pfn = -1;
    }

    for (int i = 0; i < n; i++)
    {
        frames[i].pfn = i;
        frames[i].pn = -1;
    }
    free_head = &frames[0];
    for (int i = 0; i < n; i++)
    {
        if (i < n - 1)
        {
            frames[i].next = &frames[i + 1];
        }
    }
    busy_head = NULL;
    busy_tail = NULL;
}

void display() {
    for (int i = 0; i < n; i++)
    {
        cout << "Frame " << i << ": " << "Page" << frames[i].pn << "\t";
    }
	cout << endl;
}

bool pageFault(int pn)
{
    if (pages[pn].pfn == -1)
    {
        return true;
    }
    else
    {
        hitcount++;
        display();
        return false;
    }
}

void FIFO(int pn)
{
    if (pageFault(pn))
    {
        if (free_head != NULL)
        {
            frame* temp = free_head;
            free_head = free_head->next;
            temp->pn = pn;
            pages[pn].pfn = temp->pfn;
            temp->next = NULL;
            if (busy_head)
            {
                busy_tail->next = temp;
                busy_tail = temp;
            }
            else
            {
                busy_head = temp;
                busy_tail = temp;
            }
            display();
        }
        else
        {
            frame* temp = busy_head;
            pages[temp->pn].pfn = -1;
            busy_head = busy_head->next;
            temp->pn = pn;
            pages[pn].pfn = temp->pfn;
            temp->next = NULL;
            if (busy_head)
            {
                busy_tail->next = temp;
                busy_tail = temp;
            }
            else
            {
                busy_head = temp;
                busy_tail = temp;
            }
            display();
        }
    }
}

frame* getMinTime()
{
    frame* temp = busy_head;
    frame* minf = busy_head;
    while (temp)
    {
        if (pages[temp->pn].time < pages[minf->pn].time)
        {
            minf = temp;
        }
        temp = temp->next;
    }
    return minf;
}

void LRU(int pn)
{
    cur_time++;
    pages[pn].time = cur_time;
    if (pageFault(pn)) {
        if (free_head)
        {
            frame* temp = free_head;
            free_head = free_head->next;
            temp->pn = pn;
            pages[pn].pfn = temp->pfn;
            temp->next = NULL;
            if (busy_head)
            {
                busy_tail->next = temp;
                busy_tail = temp;
            }
            else
            {
                busy_head = temp;
                busy_tail = temp;
            }
            display();
        }
        else
        {
		    pages[getMinTime()->pn].pfn = -1;
            getMinTime()->pn = pn;
            pages[pn].pfn = getMinTime()->pfn;
            display();
        }
    }
}

double hitRatio()
{
    return (double)hitcount / m;
}



int main()
{
    cout << "Enter the number of frames: ";
    cin >> n;
    frames.resize(n);
    init();
    cout << "Enter the number of pages: ";
    cin >> m;
    cout << "Enter the page number: " << endl;
    for (int i = 0; i < m; i++)
    {
        cin >> v[i];
        FIFO(v[i]);
    }
    cout << "hitRatio:" << hitRatio() << endl;
    system("pause");
}