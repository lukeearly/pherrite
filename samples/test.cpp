struct MyStruct {
    int x;
    int y;
    int z;
    int w;
    int sum() {
        return x + y + z + w + 12;
    }
};

MyStruct *abc() {
    return new MyStruct();
};

int main() {
    return abc()->sum();
}
