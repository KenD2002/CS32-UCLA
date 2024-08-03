void removeBad(vector<Movie*>& v)
{
    std::vector <Movie*>::iterator i;
    i = v.begin();
    while(i != v.end())
    {
        if((*i)->rating() < 50)
        {
            delete *i;
            i = v.erase(i);
            continue;
        }
        i ++;
    }
}
