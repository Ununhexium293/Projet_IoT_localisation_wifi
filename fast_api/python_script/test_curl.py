#decode la base 64
import base64

#code pour atoi trouvé ici : https://www.codespeedy.com/string-atoi-in-python/
def atoi(str):
    resultant = 0
    for i in range(len(str)):
        resultant = resultant * 10 + (ord(str[i]) - ord('0'))        #It is ASCII substraction 
    return resultant

# à modifier en fonction du format envoyé, moi j'ai fais d'une façon mais au moins vous avez l'idée
def base64_to_ascii(payload):
    payload_trad = base64.b64decode(payload).decode() # décode la payload recue de TTN encodé en base 64

    #récuperation du bssid et formatage pour l'API de wiggle
    bssid = ""
    for i in range (0, 6) :
        bssid = bssid + payload_trad[2*i : 2 * i + 2].upper()
        if i != 5 :
            bssid = bssid + ':'

    #récuperation du rssi
    rssi_str = payload_trad[14:16]
    rssi = - atoi(rssi_str)
    
    return [bssid, rssi]

#recupere les messages de ttn

import time
import requests
import json

def wiggle_request(BSSID) :
    headers = {
        'Accept': 'application/json',
    }

    params = {
        'netid': BSSID,
    }

    response = requests.get(
        'https://api.wigle.net/api/v2/network/detail',
        params=params,
        headers=headers,
        auth=('AIDf66bd0b82e82402e58b36f6dbdd59ff5', '497caef5f342d8155c71da9894fa6d43'),
    )

    longitude = -1
    latitude = -1

    print(response.text)

    try :
        payload = json.loads(response.text)
        success = payload["success"]
        if success == True :
            latitude = payload["results"][0]["trilat"]
            longitude = payload["results"][0]["trilong"]
    except json.decoder.JSONDecodeError :
        pass

    return [latitude, longitude]

def get_lora_message() :
        headers = {
            'Authorization': 'Bearer NNSXS.SMS4CI3XZBYCNZFDAJKBSL3EU3VUMHLVHDMKYKI.VV2CUQ7WJMTQ36W6UWUX5RYYWPW6GXP6GKEZSEPQONIP2IL3V53A',
            'Accept': 'text/event-stream',
        }

        params = {
            'last': '24h',
            'limit' : '5'
        }

        response = requests.get(
            'https://eu1.cloud.thethings.network/api/v3/as/applications/tp2-iot-app/packages/storage/uplink_message',
            params=params,
            headers=headers,
        )
        
        lines = response.text.strip().split('\n\n')

        results = []
        wiggle_results = []

        for line in lines :
            try :
                payload = json.loads(line)
                results.append(base64_to_ascii(payload["result"]["uplink_message"]["frm_payload"]))
                wiggle_results.append(wiggle_request(results[-1][0]))
            except json.decoder.JSONDecodeError :
                continue
        
        long = 0
        lat = 0
        div = 0

        print(results)
        print(wiggle_results)

        for i in range(0, 5, 1) :
            wiggle_result = wiggle_results[i]
            result = results[i]
            if wiggle_result == [-1, -1] :
                continue
            else :
                temp = 100 - result[1]
                lat += temp * wiggle_result[0]
                long += temp * wiggle_result[1]
                div += temp
        
        if (div != 0) :
            lat = lat / div
            long = long / div
        
        print([lat, long])

        
def boucle() :
    while True :
        print("begin read :")
        get_lora_message()
        print("end read\n")
        time.sleep(120)

boucle()