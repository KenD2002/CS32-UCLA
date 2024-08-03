void listAll(string path, const File* f)  // two-parameter overload
{
    std::string str = path;
    if(f->files() == nullptr) // PlainFile achieved
    {
        std::cout << str << std::endl;
        return;
    }

    // Directory:
    str += "/";
    std::cout << str << std::endl;

    for(int i = 0; i < f->files()->size(); i++)
    {
        listAll(str + f->files()->at(i)->name(), f->files()->at(i));
    }
}
