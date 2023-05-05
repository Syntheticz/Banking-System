    while(1)
    {
        // clear the console screen

        switch(mainMenu())
        {
            case 1:
                register_account(); break;
            case 2:
                login(); break;
            case 3:
                exit(0);
            default:
                printf("Invalid Input (1-3 only).\n");
                system("pause"); // wait for user input
                break;
        }
    }

    return 0;