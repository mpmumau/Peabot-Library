export const changePanelAction = (panel_name) => {
    return {
        type: "CHANGE_PANEL",
        payload: panel_name
    }
}

export const changeSpeed = (speed) => {
    return {
        type: "CHANGE_SPEED",
        payload: speed
    }
}

export const changeDistance = (distance) => {
    return {
        type: "CHANGE_DISTANCE",
        payload: distance
    }
}

export const refreshLog = () => {
    return {
        type: "REFRESH_LOG",
        payload: null
    }
}