void removeOdd(list<int>& li)
{
    std::list <int>::iterator i;
    i = li.begin();
    while(i != li.end())
    {
        if((*i) % 2 == 1)
        {
            i = li.erase(i);
            continue;
        }
        i ++;
    }
}
