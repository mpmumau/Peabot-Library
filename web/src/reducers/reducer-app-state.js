var default_state = {
    panel: "d-pad",
    speed: 0.5,
    distance: 1
};

export default function(state, action) {
    if (typeof state === 'undefined')
    {
        return default_state;
    }

    switch (action.type) {
        case "CHANGE_PANEL":
            state.panel = action.payload;
            break;
        case "CHANGE_SPEED":
            state.speed = action.payload;
            break;
        case "CHANGE_DISTANCE":
            state.distance = action.payload;
            break;
    }

    return state;
}