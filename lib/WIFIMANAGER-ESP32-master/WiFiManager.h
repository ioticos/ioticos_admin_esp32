/**************************************************************
   WiFiManager is a library for the ESP8266/Arduino platform
   (https://github.com/esp8266/Arduino) to enable easy
   configuration and reconfiguration of WiFi credentials using a Captive Portal
   inspired by:
   http://www.esp8266.com/viewtopic.php?f=29&t=2520
   https://github.com/chriscook8/esp-arduino-apboot
   https://github.com/esp8266/Arduino/tree/master/libraries/DNSServer/examples/CaptivePortalAdvanced
   Built by AlexT https://github.com/tzapu
   Licensed under MIT license
 **************************************************************/


#ifndef WiFiManager_h
#define WiFiManager_h

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <FS.h>
#else
  #include "SPIFFS.h"
  //#include <WiFi.h>
  #include <WebServer.h>
#endif

#include <DNSServer.h>
#include <memory>

#if defined(ESP8266)
extern "C" {
  #include "user_interface.h"
}
#define ESP_getChipId()   (ESP.getChipId())
#else
#include <esp_wifi.h>
#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())
#endif

const char HTTP_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\" /><meta charset=\"utf-8\" /><title>WiFi Manager</title>";
const char HTTP_STYLE[] PROGMEM           = "<style>html{ font-family:sans-serif;line-height:1.15; -ms-text-size-adjust:100%; -webkit-text-size-adjust:100%;background-color: #2E3E4E; }body{ margin:0;color:rgba(0,0,0,0.87);font-size:0.875rem;background-color: #2E3E4E; -webkit-font-smoothing:antialiased; }button{ color:inherit; }p,h2,h3{ orphans:3;widows:3; }h2,h3{ page-break-after:avoid; }h1,h2,h3,h4,h5,h6{ margin-top:0;margin-bottom: .5rem; }h1,h2,h3,h4,h5,h6, .h1, .h2, .h3, .h4, .h5, .h6{ margin-bottom:0.5rem;font-family:inherit;font-weight:500;line-height:1.1;color:inherit; }a{text-decoration:none; }h3, .h3{ font-size:1.75rem; } .center-block{ margin:0 auto;text-align:center;color:white; } .w-8{ width:8px;height:8px;display:inline-block; } .w-16{ width:16px;height:16px;display:inline-block; } .w-20{ width:20px;height:20px;display:inline-block; } .w-24{ width:24px;height:24px;display:inline-block;text-align:center; } .w-32{ width:32px;height:32px;line-height:32px;display:inline-block;text-align:center; } .w-40{ width:40px;height:40px;line-height:40px;display:inline-block;text-align:center; } .w-48{ width:48px;height:48px;line-height:48px;display:inline-block;text-align:center; } .w-56{ width:56px;height:56px;line-height:56px;display:inline-block;text-align:center; } .w-64{ width:64px; } .w-96{ width:96px; } .w-128{ width:128px; } .w-xxs{ width:60px; } .w-xs{ width:90px; } .w-sm{ width:120px; } .w{ width:180px; } .w-md{ width:200px; } .w-lg{ width:240px; } .w-xl{ width:280px; } .w-xxl{ width:320px; } .w-full{ width:100%; } .w-auto{ width:auto; } .h-auto{ height:auto; } .h-full{ height:100%; } .h-v{ height:100vh; } .h-v-5{ height:50vh; } @media(max-width:767px) { .w-auto-sm{ width:auto; } .w-full-sm{ width:100%; } } @media(max-width:575px) { .w-auto-xs{ width:auto; } .w-full-xs{ width:100%; } } .md-btn{ -webkit-user-select:none; -moz-user-select:none; -ms-user-select:none;user-select:none;position:relative;outline:none;border:0;border-radius:3px;padding:6px;display:inline-block;white-space:nowrap;text-align:center;text-transform:uppercase;font-weight:bold;font-style:inherit;font-variant:inherit;font-size:inherit;font-family:inherit;line-height:inherit;text-decoration:none;cursor:pointer;overflow:hidden;transition:box-shadow 0.4s cubic-bezier(0.25,0.8,0.25,1),background-color 0.4s cubic-bezier(0.25,0.8,0.25,1), -webkit-transform 0.4s cubic-bezier(0.25,0.8,0.25,1);transition:box-shadow 0.4s cubic-bezier(0.25,0.8,0.25,1),background-color 0.4s cubic-bezier(0.25,0.8,0.25,1),transform 0.4s cubic-bezier(0.25,0.8,0.25,1); } .md-flat{ background:transparent; } .md-btn.md-flat:not([disabled]):hover, .md-btn.md-flat:not([disabled]):focus{ background-color:rgba(158,158,158,0.2); } .md-btn-circle{ width:28px;height:28px;border-radius:50%;padding:2px; } .md-btn[disabled], .md-btn.md-raised[disabled], .md-btn.md-fab[disabled] {color:rgba(0,0,0,0.26);background-color:transparent;cursor:not-allowed; } .md-fab{ line-height:44px; } .md-fab-offset{ margin-top: -20px;margin-bottom: -20px; } .md-btn:focus{ outline:none; } .md-btn:hover{ text-decoration:none; } .md-btn.md-cornered{ border-radius:0; } .md-btn.md-icon{ padding:0;background:none; } .md-btn.md-raised{ -webkit-transform:translate3d(0,0,0);transform:translate3d(0,0,0); } .md-btn.md-fab{ z-index:20;width:56px;height:56px;border-radius:50%;border-radius:50%;overflow:hidden; -webkit-transform:translate3d(0,0,0);transform:translate3d(0,0,0);transition:0.2s linear;transition-property: -webkit-transform,box-shadow;transition-property:transform,box-shadow; } .md-btn.md-raised:not([disabled]), .md-btn.md-fab{ box-shadow:0 2px 5px 0 rgba(0,0,0,0.26); } .md-btn.md-raised:not([disabled]):focus, .md-btn.md-raised:not([disabled]):hover, .md-btn.md-fab:not([disabled]):focus, .md-btn.md-fab:not([disabled]):hover{ box-shadow:0 4px 8px 0 rgba(0,0,0,0.4); -webkit-transform:translate3d(0, -1px,0);transform:translate3d(0, -1px,0); } .md-fab-bottom-right{ top:auto;right:20px;bottom:20px;left:auto;position:absolute; } .md-fab-bottom-left{ top:auto;right:auto;bottom:20px;left:20px;position:absolute; } .md-fab-top-right{ top:20px;right:20px;bottom:auto;left:auto;position:absolute; } .md-fab-top-left{ top:20px;right:auto;bottom:auto;left:20px;position:absolute; } .md-btn.md-fab.md-mini{ width:40px;height:40px;line-height:28px; } .m-a{ margin:1rem!important; } .m-t{ margin-top:1rem!important; } .m-r{ margin-right:1rem!important; } .m-b{ margin-bottom:1rem!important; } .m-l{ margin-left:1rem!important; } .m-x{ margin-right:1rem!important;margin-left:1rem!important; } .m-y{ margin-top:1rem!important;margin-bottom:1rem!important; } .m-a-lg{ margin:3rem!important; } .m-t-lg{ margin-top:3rem!important; } .m-r-lg{ margin-right:3rem!important; } .m-b-lg{ margin-bottom:3rem!important; } .m-l-lg{ margin-left:3rem!important; } .m-x-lg{ margin-right:3rem!important;margin-left:3rem!important; } .m-y-lg{ margin-top:3rem!important;margin-bottom:3rem!important; } .m-a-md{ margin:1.5rem!important; } .m-t-md{ margin-top:1.5rem!important; } .m-r-md{ margin-right:1.5rem!important; } .m-b-md{ margin-bottom:1.5rem!important; } .m-l-md{ margin-left:1.5rem!important; } .m-x-md{ margin-right:1.5rem!important;margin-left:1.5rem!important; } .m-y-md{ margin-top:1.5rem!important;margin-bottom:1.5rem!important; } .m-a-sm{ margin:0.5rem!important; } .m-t-sm{ margin-top:0.5rem!important; } .m-r-sm{ margin-right:0.5rem!important; } .m-b-sm{ margin-bottom:0.5rem!important; } .m-l-sm{ margin-left:0.5rem!important; } .m-x-sm{ margin-right:0.5rem!important;margin-left:0.5rem!important; } .m-y-sm{ margin-top:0.5rem!important;margin-bottom:0.5rem!important; } .m-a-xs{ margin:0.25rem!important; } .m-t-xs{ margin-top:0.25rem!important; } .m-r-xs{ margin-right:0.25rem!important; } .m-b-xs{ margin-bottom:0.25rem!important; } .m-l-xs{ margin-left:0.25rem!important; } .m-x-xs{ margin-right:0.25rem!important;margin-left:0.25rem!important; } .m-y-xs{ margin-top:0.25rem!important;margin-bottom:0.25rem!important; } .p-a{ padding:1rem!important; } .p-t{ padding-top:1rem!important; } .p-r{ padding-right:1rem!important; } .p-b{ padding-bottom:1rem!important; } .p-l{ padding-left:1rem!important; } .p-x{ padding-right:1rem!important;padding-left:1rem!important; } .p-y{ padding-top:1rem!important;padding-bottom:1rem!important; } .p-a-lg{ padding:3rem!important; } .p-t-lg{ padding-top:3rem!important; } .p-r-lg{ padding-right:3rem!important; } .p-b-lg{ padding-bottom:3rem!important; } .p-l-lg{ padding-left:3rem!important; } .p-x-lg{ padding-right:3rem!important;padding-left:3rem!important; } .p-y-lg{ padding-top:3rem!important;padding-bottom:3rem!important; } .p-a-md{ padding:1.5rem!important; } .p-t-md{ padding-top:1.5rem!important; } .p-r-md{ padding-right:1.5rem!important; } .p-b-md{ padding-bottom:1.5rem!important; } .p-l-md{ padding-left:1.5rem!important; } .p-x-md{ padding-right:1.5rem!important;padding-left:1.5rem!important; } .p-y-md{ padding-top:1.5rem!important;padding-bottom:1.5rem!important; } .p-a-sm{ padding:0.5rem!important; } .p-t-sm{ padding-top:0.5rem!important; } .p-r-sm{ padding-right:0.5rem!important; } .p-b-sm{ padding-bottom:0.5rem!important; } .p-l-sm{ padding-left:0.5rem!important; } .p-x-sm{ padding-right:0.5rem!important;padding-left:0.5rem!important; } .p-y-sm{ padding-top:0.5rem!important;padding-bottom:0.5rem!important; } .p-a-xs{ padding:0.25rem!important; } .p-t-xs{ padding-top:0.25rem!important; } .p-r-xs{ padding-right:0.25rem!important; } .p-b-xs{ padding-bottom:0.25rem!important; } .p-l-xs{ padding-left:0.25rem!important; } .p-x-xs{ padding-right:0.25rem!important;padding-left:0.25rem!important; } .p-y-xs{ padding-top:0.25rem!important;padding-bottom:0.25rem!important; } .padding{ padding:1.5rem 1.5rem; } .margin{ margin-bottom:1.5rem; } .padding-out{ margin: -1.5rem-1.5rem; } @media(max-width:991px) { .padding{ padding:1rem 1rem; } .padding-out{ margin: -1rem-1rem; } .margin{ margin-bottom:1rem; } .box, .box-color{ margin-bottom:1rem; } } @media(max-width:767px) { .padding{ padding:0.5rem 0.5rem; } .padding-out{ margin: -0.5rem-0.5rem; } .margin{ margin-bottom:0.5rem; } .box, .box-color{ margin-bottom:0.5rem; } .no-padding-xs{ padding:0; } } .no-radius{ border-radius:0 !important; } .no-r-t{ border-top-right-radius:0;border-top-left-radius:0; } .no-r-r{ border-bottom-right-radius:0;border-top-right-radius:0; } .no-r-b{ border-bottom-right-radius:0;border-bottom-left-radius:0; } .no-r-l{ border-bottom-left-radius:0;border-top-left-radius:0; } .r{ border-radius:3px; } .r-t{ border-top-right-radius:3px;border-top-left-radius:3px; } .r-r{ border-bottom-right-radius:3px;border-top-right-radius:3px; } .r-b{ border-bottom-right-radius:3px;border-bottom-left-radius:3px; } .r-l{ border-bottom-left-radius:3px;border-top-left-radius:3px; } .r-2x{ border-radius:6px; } .r-3x{ border-radius:9px; } .navbar{ background:inherit;min-height:3.5rem; } .navbar.up{ margin:0 -0.6em; } .navbar.avatar{ top: -2px; } .navbar-md{ min-height:4rem;padding-top:0.75rem;padding-bottom:0.75rem; } .navbar-md.navbar{ min-height:auto;padding-top:0;padding-bottom:0; } .navbar-sm{ min-height:3rem;padding-top:0.25rem;padding-bottom:0.25rem; } .navbar-sm.navbar{ min-height:auto;padding-top:0;padding-bottom:0; } .navbar-brand{ font-weight:bold; } .navbar-brand:hover{ text-decoration:none; } .navbar-brand img, .navbar-brand svg{ max-height:24px;vertical-align: -4px;display:inline-block; } .navbar-brand> span{ display:inline-block;line-height:0.8;margin-left:10px; } .navbar-brand.md img, .navbar-brand.md svg{ max-height:32px;vertical-align: -8px; } .navbar-brand.lg img, .navbar-brand.lg svg{ max-height:48px;vertical-align: -16px; } .pull-center.navbar-brand{ margin-left: -50%;float:left; } .navbar-collapse.collapsing, .navbar-collapse.collapse.show{ background:inherit;left:0;top:100%;max-height:100vh;overflow:auto;width:100%; } .navbar-collapse.collapsing.nav-active-border, .navbar-collapse.collapse.show.nav-active-border{ border-color:transparent; } @media(max-width:575px) { .navbar-toggleable.navbar-collapse.collapsing, .navbar-toggleable.navbar-collapse.collapse.show{ padding:0 1rem 4rem 1rem;position:absolute;z-index:1040; } } @media(max-width:767px) { .navbar-toggleable-sm.navbar-collapse.collapsing, .navbar-toggleable-sm.navbar-collapse.collapse.show{ padding:0 1rem 4rem 1rem;position:absolute;z-index:1040; } } @media(max-width:991px) { .navbar-toggleable-md.navbar-collapse.collapsing, .navbar-toggleable-md.navbar-collapse.collapse.show{ padding:0 1rem 4rem 1rem;position:absolute;z-index:1040; } } @media(max-width:1199px) { .navbar-toggleable-lg.navbar-collapse.collapsing, .navbar-toggleable-lg.navbar-collapse.collapse.show{ padding:0 1rem 4rem 1rem;position:absolute;z-index:1040; } } .navbar-toggleable-xl.navbar-collapse.collapsing, .navbar-toggleable-xl.navbar-collapse.collapse.show{ padding:0 1rem 4rem 1rem;position:absolute;z-index:1040; } .box, .box-color{ background-color: #fff;position:relative;margin-bottom:1.5rem; } .box-shadow-z1, .box-shadow-z1.box, .box-shadow-z1.box-color{ box-shadow:0 1px 2px rgba(0,0,0,0.1),0 -1px 0px rgba(0,0,0,0.02); } .text-color{ color:rgba(0,0,0,0.87); } .text-sm{ font-size:0.8rem; } .md-form-group{ padding:18px 0 24px 0;position:relative; } .md-input{ background:transparent;position:relative;z-index:5;border:0;box-shadow:none;border-radius:0;border-bottom:1px solid rgba(160,160,160,0.2);width:100%;height:34px;padding:2px;color:inherit; } .md-input:focus, .md-input.focus{ border-color: #0cc2aa;border-bottom-width:2px;padding-bottom:1px; } .md-input:focus~ label, .md-input.focus~ label{ color: #0cc2aa;opacity:1;top:0 !important;font-size:0.85em!important; } .float-label.md-input~ label{ top:20px;font-size:1em; } .float-label.md-input.ng-dirty~ label, .float-label.md-input.has-value~ label{ top:0;font-size:0.85em; } .md-input~ label{ -webkit-transition:all 0.2s;transition:all 0.2s;font-size:0.85em;position:absolute;z-index:0;opacity:0.5;display:inline-block;top:0px;left:0; } .md-input.disabled, .md-input[disabled] {opacity:0.5; }textarea.md-input{ height:auto; } .md-input-white:focus, .md-input-white.focus{ border-color: #fff; } .md-input-white:focus~ label, .md-input-white.focus~ label{ color: #fff; } .md-input-msg{ position:absolute;bottom:0;line-height:24px;font-size:0.85em; } .md-input-msg.right{ right:0; } .btn.label{ position:relative;top: -1px; } .btn{ font-weight:500;outline:0 !important; } .btn:not([disabled]):hover, .btn:not([disabled]):focus, .btn:not([disabled]).active{ box-shadow:inset 0-10rem 0px rgba(158,158,158,0.2); } .btn> i.pull-left, .btn> i.pull-right{ line-height:1.5; } .btn.rounded{ padding-left:1.2em;padding-right:1.2em; } .btn-md{ padding: .4695rem.75rem;font-size:0.9rem; } .btn-xs{ padding: .2195rem.5rem;font-size:0.8rem; } .btn-outline{ border-width:1px;background-color:transparent;box-shadow:none!important; } .btn.b-primary:hover, .btn.b-primary:focus, .btn.primary:hover, .btn.primary:focus{ color:rgba(255,255,255,0.87) !important;background-color:grey;cursor:pointer; } .btn{ display:inline-block;font-weight:normal;line-height:1.25;text-align:center;white-space:nowrap;vertical-align:middle; -webkit-user-select:none; -moz-user-select:none; -ms-user-select:none;user-select:none;border:1px solid transparent;padding:0.5rem 1rem;font-size:1rem;border-radius:0.25rem; -webkit-transition:all 0.2s ease-in-out; -o-transition:all 0.2s ease-in-out;transition:all 0.2s ease-in-out; } .btn:focus, .btn:hover{ text-decoration:none; } .btn:focus, .btn.focus{ outline:0; -webkit-box-shadow:0 0 0 2px rgba(2,117,216,0.25);box-shadow:0 0 0 2px rgba(2,117,216,0.25); } .btn.disabled, .btn:disabled{ cursor:not-allowed;opacity: .65; } .btn:active, .btn.active{ background-image:none; }a.btn.disabled,fieldset[disabled]a.btn{ pointer-events:none; } .btn-primary{ color: #fff;background-color: #0275d8;border-color: #0275d8; } .primary{ color:rgba(255,255,255,0.87);background-color: #0cc2aa; } .primary.lt{ background-color: #0dceb5; } .primary.dk{ background-color: #0bb69f; } .primary.bg{ background-color: #0cc2aa; } .btn-block{ display:block;width:100%; } .btn-block+ .btn-block{ margin-top:0.5rem; }input[type=\"submit\"].btn-block,input[type=\"reset\"].btn-block,input[type=\"button\"].btn-block{ width:100%; } .enlace{color: #0cc2aa;font-size:1rem; } .etiqueta{color:black; } </style>";
const char HTTP_SCRIPT[] PROGMEM          = "<script>function c(l) {document.getElementById('s').value= l.innerText||l.textContent;document.getElementById('p').focus(); } </script>";
const char HTTP_HEAD_END[] PROGMEM        = "</head><body><div class=\"center-block w-xxl w-auto-xs p-y-md\"><div class=\"navbar\"><h3 class=\"center-block\">Panel de Configuración</h3></div>";
const char HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\"method=\"get\"><button class=\"btn primary btn-block p-x-md\">Conexión/Redes Disponibles</button></form><br/><form action=\"/0wifi\"method=\"get\"><button class=\"btn primary btn-block p-x-md\" >Conexión</button></form><br/><form action=\"/i\"method=\"get\"><button class=\"btn primary btn-block p-x-md\" >Dispositivo</button></form><br/><form action=\"/r\"method=\"get\"><button class=\"btn primary btn-block p-x-md\" >Reset</button><br/></form><form action=\"/custom\"method=\"get\"><button class=\"btn primary btn-block p-x-md\" >Valores MQTT</button></form>";
const char HTTP_PORTAL_INFO[] PROGMEM     = "<form><div class=\"md-form-group\"><input class=\"md-input\"placeholder=\"{infoValue}\"readonly><label>{info}</label><br/></div></form>";
const char HTTP_ITEM[] PROGMEM            = "<div><a class=\"enlace\"href='#p'onclick='c(this)'>{v}</a>&nbsp; <span class='q{i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<div class=\"p-a-md box-color r box-shadow-z1 text-color m-a\"><form method='post'action='wifisave'><div class=\"m-b text-sm\">Configuración</div>";
const char HTTP_WIFI[] PROGMEM            = "<div class=\"md-form-group\"><input class=\"md-input\"id='s'name='s'length=32><label>SSID</label></div><div class=\"md-form-group\"><input class=\"md-input\"id='p'name='p'length=64 type='password'><label>Pass</label></div>";
const char HTTP_FORM_PARAM[] PROGMEM      = "";
const char HTTP_FORM_PARAMETER[] PROGMEM  = "<div class=\"md-form-group\"><input class=\"md-input\"id='{p}'name='{p}'length=64 type='text'placeholder='{vp}'><label for=\"{p}\">{p}</label></div>";
const char HTTP_FORM_END[] PROGMEM        = "<br/><button class=\"btn primary btn-block p-x-md\"type='submit'>Guardar</button></form></div>";
const char HTTP_SCAN_LINK[] PROGMEM       = "<br/><div class=\"\"><a class=\"enlace\"href=\"/wifi\">Scan</a></div>";
const char HTTP_SAVED[] PROGMEM           = "<h4>Valores guardados</h3>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";

#define WIFI_MANAGER_MAX_PARAMS 10

class WiFiManagerParameter {
  public:
    WiFiManagerParameter(const char *custom);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length);
    WiFiManagerParameter(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    const char *getID();
    const char *getValue();
    const char *getPlaceholder();
    int         getValueLength();
    const char *getCustomHTML();
  private:
    const char *_id;
    const char *_placeholder;
    char       *_value;
    int         _length;
    const char *_customHTML;

    void init(const char *id, const char *placeholder, const char *defaultValue, int length, const char *custom);

    friend class WiFiManager;
};


class WiFiManager
{
  public:
    WiFiManager();

    boolean       autoConnect();
    boolean       autoConnect(char const *apName, char const *apPassword = NULL);

    //if you want to always start the config portal, without trying to connect first
    boolean       startConfigPortal();
    boolean       startConfigPortal(char const *apName, char const *apPassword = NULL);

    // get the AP name of the config portal, so it can be used in the callback
    String        getConfigPortalSSID();
    String        getSSID();
    String        getPassword();
    void          resetSettings();

    //sets timeout before webserver loop ends and exits even if there has been no setup.
    //useful for devices that failed to connect at some point and got stuck in a webserver loop
    //in seconds setConfigPortalTimeout is a new name for setTimeout
    void          setConfigPortalTimeout(unsigned long seconds);
    void          setTimeout(unsigned long seconds);

    //sets timeout for which to attempt connecting, useful if you get a lot of failed connects
    void          setConnectTimeout(unsigned long seconds);


    void          setDebugOutput(boolean debug);
    //defaults to not showing anything under 8% signal quality if called
    void          setMinimumSignalQuality(int quality = 8);
    //sets a custom ip /gateway /subnet configuration
    void          setAPStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //sets config for a static IP
    void          setSTAStaticIPConfig(IPAddress ip, IPAddress gw, IPAddress sn);
    //called when AP mode and config portal is started
    void          setAPCallback( void (*func)(WiFiManager*) );
    //called when settings have been changed and connection was successful
    void          setSaveConfigCallback( void (*func)(void) );
    //adds a custom parameter
    void          addParameter(WiFiManagerParameter *p);
    //if this is set, it will exit after config, even if connection is unsuccessful.
    void          setBreakAfterConfig(boolean shouldBreak);
    //if this is set, try WPS setup when starting (this will delay config portal for up to 2 mins)
    //TODO
    //if this is set, customise style
    void          setCustomHeadElement(const char* element);
    //if this is true, remove duplicated Access Points - defaut true
    void          setRemoveDuplicateAPs(boolean removeDuplicates);

  private:
    std::unique_ptr<DNSServer>        dnsServer;
#ifdef ESP8266
    std::unique_ptr<ESP8266WebServer> server;
#else
    std::unique_ptr<WebServer>        server;
#endif

    //const int     WM_DONE                 = 0;
    //const int     WM_WAIT                 = 10;

    /*const String  HTTP_HEAD = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/><title>{v}</title>";*/

    void          setupConfigPortal();
    void          startWPS();

    const char*   _apName                 = "no-net";
    const char*   _apPassword             = NULL;
    String        _ssid                   = "";
    String        _pass                   = "";
    unsigned long _configPortalTimeout    = 0;
    unsigned long _connectTimeout         = 0;
    unsigned long _configPortalStart      = 0;

    String        newParametro1                = "";
    String        newParametro2               = "";
    String        newParametro3               = "";
    String        newParametro4               = "";
    String        newParametro5               = "";
    String        newParametro6               = "";
    char SPIFFSparametro1 [40];
    char SPIFFSparametro2 [40];
    char SPIFFSparametro3 [40];
    char SPIFFSparametro4 [40];
    char SPIFFSparametro5 [40];
    char SPIFFSparametro6 [40];

    IPAddress     _ap_static_ip;
    IPAddress     _ap_static_gw;
    IPAddress     _ap_static_sn;
    IPAddress     _sta_static_ip;
    IPAddress     _sta_static_gw;
    IPAddress     _sta_static_sn;

    int           _paramsCount            = 0;
    int           _minimumQuality         = -1;
    boolean       _removeDuplicateAPs     = true;
    boolean       _shouldBreakAfterConfig = false;
    boolean       _tryWPS                 = false;

    const char*   _customHeadElement      = "";

    //String        getEEPROMString(int start, int len);
    //void          setEEPROMString(int start, int len, String string);

    int           status = WL_IDLE_STATUS;
    int           connectWifi(String ssid, String pass);
    uint8_t       waitForConnectResult();

    void          handleRoot();
    void          handleWifi(boolean scan);
    void          handleWifiSave();
    void          handleInfo();
    void          handleReset();
    void          handleNotFound();
    void          handle204();
    boolean       captivePortal();
    boolean       configPortalHasTimeout();
    void          handleParameters();

    // DNS server
    const byte    DNS_PORT = 53;

    //helpers
    int           getRSSIasQuality(int RSSI);
    boolean       isIp(String str);
    String        toStringIp(IPAddress ip);

    boolean       connect;
    boolean       _debug = true;

    void (*_apcallback)(WiFiManager*) = NULL;
    void (*_savecallback)(void) = NULL;

    WiFiManagerParameter* _params[WIFI_MANAGER_MAX_PARAMS];

    template <typename Generic>
    void          DEBUG_WM(Generic text);

    template <class T>
    auto optionalIPFromString(T *obj, const char *s) -> decltype(  obj->fromString(s)  ) {
      return  obj->fromString(s);
    }
    auto optionalIPFromString(...) -> bool {
      DEBUG_WM("NO fromString METHOD ON IPAddress, you need ESP8266 core 2.1.0 or newer for Custom IP configuration to work.");
      return false;
    }
};

#endif
