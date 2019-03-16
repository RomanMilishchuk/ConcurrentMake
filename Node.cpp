#include <fstream>
#include "Node.h"

Node::Node(std::string node_name) : session(), is_connected(false) {
    session.setOption(SSH_OPTIONS_HOST, node_name.c_str());
};

Node::~Node() {
    if (is_connected) {
        session.disconnect();
    }
}

void Node::connect() {
    session.connect();
    is_connected = true;
    if(session.userauthPublickeyAuto() != SSH_AUTH_SUCCESS) {
        throw std::runtime_error("Didn't auth");
    }
}


std::string Node::execute_command(std::string cmd) {
    ssh::Channel channel(session);
    channel.openSession();
    ssh_channel_request_pty(channel.getCChannel());
    channel.requestExec(cmd.c_str());
    int nbytes;
    char buffer[256];
    std::string ret;
    nbytes = channel.read(buffer, sizeof(buffer), false, -1);
    while (nbytes > 0) {
        ret.append(buffer);
        nbytes = ssh_channel_read(channel.getCChannel(), buffer, sizeof(buffer), false);
    }
    return ret;
}

void Node::scp_write_file(std::string filename, std::string text) {
    Scp scp(session, SSH_SCP_WRITE, ".");
    scp.push_file(filename.c_str(), text.length(), S_IRUSR | S_IWUSR);
    scp.write(text);
}

std::string Node::scp_read_file(std::string filename) {
    Scp scp(session, SSH_SCP_READ, filename);

    if (scp.pull_request() != SSH_SCP_REQUEST_NEWFILE) {
        throw std::runtime_error("Invalid request from scp");
    }

    scp.accept_request();
    std::string file = scp.read();

    return file;
}

void Node::scp_copy_file_from_user(std::string from, std::string to){
    std::ifstream input(from, std::ios::binary);
    if(input.is_open()){
        input.seekg(0, std::ios::end);
        std::ios::pos_type buf_size = input.tellg();
        input.seekg(0);

        char* buf = new char[buf_size];
        input.read(buf, buf_size);
        std::string output(buf);
        free(buf);
        Node::scp_write_file(std::move(to), output);
    }
    input.close();
};

void Node::scp_copy_file_from_server(std::string from, std::string to){
    std::string input = Node::scp_read_file(from);
    std::ofstream outfile(to, std::ios::binary);
    outfile << input;
    outfile.close();
}


