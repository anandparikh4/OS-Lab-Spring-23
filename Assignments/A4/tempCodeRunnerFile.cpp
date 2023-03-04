thread_t userSimulator_thread;
    // if(pthread_create(&userSimulator_thread, NULL, userSimulator,NULL)<0) {
    //     cout << "Error in creating userSimulator thread" << endl;
    //     exit(0);
    // }    
    // pthread_join(userSimulator_thread, NULL);
    // // Check if the wall of a user has been updated
    // for(auto it=users.begin(); it!=users.end(); it++) {
    //     if(it->second.wall.size() > 0) {
    //         cout << "Wall of user " << it->first << " has been updated" << endl;
    //     }
    // }