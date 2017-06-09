import React from 'react';

var default_state = {
    log_lines: [ "No Log lines retrieved."]
};

export default function(state, action) {
    if (typeof state === 'undefined')
    {
        return default_state;
    }

    switch (action.type) {
        case "REFRESH_LOG":
            var url = "http://ML_DEVNET_PIBOT:9976/log/get"
            fetch(url, {
                method: 'GET',
            })
            .then((response) => response.json())
            .then((responseJson) => {
                state.log_lines = state.log_lines.concat(responseJson.log_lines);
            });

            break;
    }

    return state;
}