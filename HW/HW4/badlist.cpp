void removeBad(list<Movie*>& li)
{
    std::list <Movie*>::iterator i;
    i = li.begin();
    while(i != li.end())
    {
        if((*i)->rating() < 50)
        {
            delete *i;
            i = li.erase(i);
            continue;
        }
        i ++;
    }
}
