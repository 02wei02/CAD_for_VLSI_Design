int main(int argc, char *argv[]){
    PCKT c;
    
    c = input_file(argv[1]);

    output_file(argv[2], c);

    free(c);

    return 0;

}