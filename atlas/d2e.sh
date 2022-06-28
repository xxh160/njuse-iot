CLS=$1
EDGX_ADDR=$2
DEVICE=$3
VALUEDESCRIPTOR=$4
CORE_DATA="$EDGX_ADDR:48080/api/v1"

curl -X POST \
	 -d "{\"device\": \"$DEVICE\", \"readings\":[{\"name\":\"$VALUEDESCRIPTOR\",\"value\":\"$CLS\"}]}" \
	 "$CORE_DATA/event"