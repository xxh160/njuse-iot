SERVER_ADDR=$1
SERVER_PORT=$2
EDGEX_ADDR=$3
PROFILE_PATH=$4
RULE_PATH=$5

CORE_METADATA="$EDGEX_ADDR:48081/api/v1"
CORE_DATA="$EDGEX_ADDR:48080/api/v1"
CORE_COMMAND="$EDGEX_ADDR:48082/api/v1"
KUIPER="$EDGEX_ADDR:48075"

ADDRESSABLE="atlas_addr"
VALUEDESCRIPTOR="idx"
PROFILE="atlas_profile"
DEVICE="atlas"
DEVICE_SERVICE="atlas_dummy_service"

# addressable
curl -X POST \
	 -d "{\"name\":\"$ADDRESSABLE\",\"protocol\":\"HTTP\",\"address\":\"$SERVER_ADDR\", \"port\":$SERVER_PORT,\"path\":\"/register\",\"publisher\":\"none\",\"user\":\"none\",\"password\":\"none\",\"topic\":\"none\"}" \
	 "$CORE_METADATA/addressable"

# value descriptor
curl -X POST \
     -d "{\"name\": \"$VALUEDESCRIPTOR\",\"description\":\"Both rubbish type idx and led idx: 0, 1, 2, 3\",\"min\":\"0\",\"max\":\"100\",\"type\":\"Int16\",\"uomLabel\":\"count\",\"defaultValue\":\"4\",\"formatting\":\"%s\",\"labels\":[\"camera\", \"led\"]}" \
	 "$CORE_DATA/valuedescriptor"

# device profile
curl -F "file=@$PROFILE_PATH" "$CORE_METADATA/deviceprofile/uploadfile"

# device service
curl -X POST \
     -d "{\"name\":\"$DEVICE_SERVICE\",\"description\":\"dummy device service\",\"labels\":[\"camera\",\"led\"],\"adminState\":\"unlocked\",\"operatingState\":\"enabled\",\"addressable\":  {\"name\":\"$ADDRESSABLE\"}}" \
	 "$CORE_METADATA/deviceservice"

# device
curl -X POST \
     -d "{ \"name\":\"$DEVICE\", \"description\":\"Atlas200DK\", \"adminState\":\"unlocked\", \"operatingState\":\"enabled\",\"protocols\":{\"camera protocol\":{\"camera address\":\"camera 1\"}},\"labels\": [\"camera\",\"counter\"],\"location\":\"\",\"service\":{\"name\":\"$DEVICE_SERVICE\"},\"profile\":{\"name\":\"$PROFILE\"}}" \
	 "$CORE_METADATA/device"

# stream
curl -X POST \
	 -H 'Content-Type: application/json' \
     -d '{"sql": "create stream demo() WITH (FORMAT=\"JSON\", TYPE=\"edgex\")"}' \
	 "$KUIPER/streams"

# rule
LED_CMD="led"
URL=$(curl $CORE_COMMAND/device/name/$DEVICE | jq -r ".commands[] | select(.name == \"$LED_CMD\") | .get.url")
for IDX in 0 1 2 3 4
do
	bash $RULE_PATH \
		 $KUIPER \
		 $VALUEDESCRIPTOR \
		 $IDX \
		 $URL
done

