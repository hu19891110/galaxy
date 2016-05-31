// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "galaxy_res_action.h"

DEFINE_string(f, "", "specify config file");
DEFINE_string(i, "", "specify container id");
DEFINE_string(p, "", "specify agent pool");
DEFINE_string(e, "", "specify agent endpoint");
DEFINE_string(t, "", "specify agent tag or token");
DEFINE_string(u, "", "username");
DEFINE_string(o, "", "operation");
DEFINE_string(a, "", "authority, split by ,");

DECLARE_string(flagfile);

const std::string kGalaxyUsage = "galaxy_res_client.\n"
                                 "Usage:\n"
                                 "  container usage:\n"
                                 "      galaxy_res_client create_container -f <jobconfig>\n"
                                 "      galaxy_res_client update_container -f <jobconfig> -i id\n"
                                 "      galaxy_res_client remove_container -i id\n"
                                 "      galaxy_res_client list_containers\n"
                                 "      galaxy_res_client show_container -i id\n\n"
                                 "  agent usage:\n"
                                 "      galaxy_res_client add_agent -p pool -e endpoint\n"
                                 "      galaxy_res_client set_agent -p pool -e endpoint\n"
                                 "      galaxy_res_client show_agent -e endpoint\n"
                                 "      galaxy_res_client remove_agent -e endpoint\n"
                                 "      galaxy_res_client list_agents [-p pool -t tag -o cpu,mem,volums]\n"
                                 "      galaxy_res_client online_agent -e endpoint\n"
                                 "      galaxy_res_client offline_agent -e endpoint\n\n"
                                 "      galaxy_res_client preempt -i container_group_id -e endpoint\n\n"
                                 "  safemode usage:\n"
                                 "      galaxy_res_client enter_safemode\n"
                                 "      galaxy_res_client leave_safemode\n\n"
                                 "  status usage:\n"
                                 "      galaxy_res_client status\n\n"
                                 "  tag usage:\n"
                                 "      galaxy_res_client create_tag -t tag -f endpoint_file\n"
                                 "      galaxy_res_client list_tags -e endpoint\n"
                                 "  pool usage:\n"
                                 "      galaxy_res_client list_pools -e endpoint\n\n"
                                 "  user usage:\n"
                                 "      galaxy_res_client add_user -u user -t token\n"
                                 "      galaxy_res_client remove_user -u user -t token\n"
                                 "      galaxy_res_client list_users\n"
                                 "      galaxy_res_client show_user -u user\n"
                                 "      galaxy_res_client grant_user -u user -t token -p pool -o [add/remove/set/clear]\n" 
                                 "                                   -a [create_container,remove_container,update_container,\n"
                                 "                                   list_containers,submit_job,remove_job,update_job,list_jobs] \n"
                                 "Options: \n"
                                 "      -f specify config file, job config file or label config file.\n"
                                 "      -i specify container id.\n"
                                 "      -p specify agent pool.\n"
                                 "      -e specify endpoint.\n"
                                 "      -u specity user.\n"
                                 "      -t specify agent tag or token.\n"
                                 "      -o specify operation [cpu,mem,volums].\n"
                                 "      -a specify authority split by ,\n";


int main(int argc, char** argv) {
    bool ok = true;
    FLAGS_flagfile = "./galaxy.flag";
    ::google::SetUsageMessage(kGalaxyUsage);
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    if (argc < 2) {
        fprintf(stderr, "%s", kGalaxyUsage.c_str());
        return -1;
    }

    ::baidu::galaxy::client::ResAction* resAction = new 
                ::baidu::galaxy::client::ResAction();
    if (strcmp(argv[1], "create_container") == 0) {
        if (FLAGS_f.empty()) {
            fprintf(stderr, "-f is needed\n");
            return -1;
        }
        ok = resAction->CreateContainerGroup(FLAGS_f);
    } else if (strcmp(argv[1], "update_container") == 0) {
        if (FLAGS_f.empty()) {
            fprintf(stderr, "-f is needed\n");
            return -1;
        }
        
        if (FLAGS_i.empty()) {
            fprintf(stderr, "-i is needed\n");
            return -1;
        }

        ok = resAction->UpdateContainerGroup(FLAGS_f, FLAGS_i);
    } else if (strcmp(argv[1], "remove_container") == 0) {
        if (FLAGS_i.empty()) {
            fprintf(stderr, "-i is needed\n");
            return -1;
        }
        ok = resAction->RemoveContainerGroup(FLAGS_i);

    } else if (strcmp(argv[1], "list_containers") == 0) {
        ok = resAction->ListContainerGroups();
    } else if (strcmp(argv[1], "show_container") == 0) {
        if (FLAGS_i.empty()) {
            fprintf(stderr, "-i is needed\n");
            return -1;
        }
        ok = resAction->ShowContainerGroup(FLAGS_i);

    } else if (strcmp(argv[1], "set_agent") == 0) {
        //已有agent,重置pool
        if (FLAGS_p.empty()) {
            fprintf(stderr, "-p is needed\n");
            return -1;
        }
        if (FLAGS_e.empty()) {
            fprintf(stderr, "-e is needed\n");
            return -1;
        }
        ok =  resAction->AddAgent(FLAGS_p, FLAGS_e);
    } else if (strcmp(argv[1], "add_agent") == 0) { 
        if (FLAGS_p.empty()) {
            fprintf(stderr, "-p is needed\n");
            return -1;
        }
        if (FLAGS_e.empty()) {
            fprintf(stderr, "-e is needed\n");
            return -1;
        }
        ok =  resAction->AddAgentToPool(FLAGS_e, FLAGS_p);
    } else if (strcmp(argv[1], "show_agent") == 0) { 
        if (FLAGS_e.empty()) {
            fprintf(stderr, "-e is needed\n");
            return -1;
        }
        ok = resAction->ShowAgent(FLAGS_e);
    } else if (strcmp(argv[1], "remove_agent") == 0) {
        if (FLAGS_e.empty()) {
            fprintf(stderr, "-e is needed\n");
            return -1;
        }
        ok = resAction->RemoveAgent(FLAGS_e);
    } else if (strcmp(argv[1], "list_agents") == 0) {
        if (!FLAGS_p.empty()) {
            ok = resAction->ListAgentsByPool(FLAGS_p, FLAGS_o);
        } else if (!FLAGS_t.empty()) {
            ok = resAction->ListAgentsByTag(FLAGS_t, FLAGS_o);
        } else {
            ok = resAction->ListAgents(FLAGS_o);
        }
    } else if (strcmp(argv[1], "enter_safemode") == 0) { 
        ok =  resAction->EnterSafeMode();
    } else if (strcmp(argv[1], "leave_safemode") == 0) {
        ok = resAction->LeaveSafeMode();
    } else if (strcmp(argv[1], "online_agent") == 0) {
        if (FLAGS_e.empty()) {
            fprintf(stderr, "-e is needed\n");
            return -1;
        }

        ok = resAction->OnlineAgent(FLAGS_e);
    } else if (strcmp(argv[1], "offline_agent") == 0) {
        if (FLAGS_e.empty()) {
            fprintf(stderr, "-e is needed\n");
            return -1;
        }
        ok = resAction->OfflineAgent(FLAGS_e);
    } else if (strcmp(argv[1], "preempt") == 0) {
        if (FLAGS_e.empty()) {
            fprintf(stderr, "-e is needed\n");
            return -1;
        }
        if (FLAGS_i.empty()) {
            fprintf(stderr, "-i is needed\n");
            return -1;
        }
        ok = resAction->Preempt(FLAGS_i, FLAGS_e);
    } else if (strcmp(argv[1], "status") == 0) {
        ok = resAction->Status();
    } else if (strcmp(argv[1], "create_tag") == 0) { 
        if (FLAGS_t.empty() || FLAGS_f.empty()) {
            fprintf(stderr, "-t and -f needed\n");
            return -1;
        }
        ok = resAction->CreateTag(FLAGS_t, FLAGS_f);
    } else if (strcmp(argv[1], "list_tags") == 0) {
        if (!FLAGS_e.empty()) {
            ok = resAction->GetTagsByAgent(FLAGS_e);
        } else {
            ok = resAction->ListTags();
        }
    } else if (strcmp(argv[1], "list_pools") == 0) {
        if (FLAGS_e.empty()) {
            fprintf(stderr, "-e is needed\n");
            return -1;
        }
        ok = resAction->GetPoolByAgent(FLAGS_e);
    } else if (strcmp(argv[1], "add_user") == 0) { 
        if (FLAGS_u.empty() || FLAGS_t.empty()) {
            fprintf(stderr, "-u and -t are needed\n");
            return -1;
        }
        ok = resAction->AddUser(FLAGS_u, FLAGS_t);
    } else if (strcmp(argv[1], "remove_user") == 0) { 
        if (FLAGS_u.empty() || FLAGS_t.empty()) {
            fprintf(stderr, "-u and -t are needed\n");
            return -1;
        }
        ok = resAction->RemoveUser(FLAGS_u, FLAGS_t);
    } else if (strcmp(argv[1], "list_users") == 0) {
        ok = resAction->ListUsers();
    } else if (strcmp(argv[1], "show_user") == 0) {
        if (FLAGS_u.empty()) {
            fprintf(stderr, "-u\n");
            return -1;
        }
        ok = resAction->ShowUser(FLAGS_u);
    } else if (strcmp(argv[1], "grant_user") == 0) {
        if (FLAGS_u.empty() || FLAGS_t.empty() || FLAGS_p.empty()) {
            fprintf(stderr, "-u, -t and -p are needed\n");
            return -1;
        }
        
        if (FLAGS_a.empty() || FLAGS_o.empty()) {
            fprintf(stderr, "-a and -o are needed\n");
            return -1;
        }

        ok =  resAction->GrantUser(FLAGS_u, FLAGS_t, FLAGS_p, FLAGS_o, FLAGS_a);

    } else {
        fprintf(stderr, "%s", kGalaxyUsage.c_str());
        return -1;
    }

    if (ok) {
        return 0;
    } else {
        return -1;
    }
}

/* vim: set ts=4 sw=4 sts=4 tw=100 */
