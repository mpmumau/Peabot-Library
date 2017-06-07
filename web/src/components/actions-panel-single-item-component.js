import React, {Component} from 'react';

class ActionsPanelSingleItemComponent extends Component {
    constructor(props) {
        super(props);

        this.robot_url = "http://ML_DEVNET_PIBOT:9976/"

        this.doAction = this.doAction.bind(this);
    }

    doAction(action_name) {
        var action;
        var reverse;

        switch (action_name) {
            case "elevate_out":
                action = "elevate";
                reverse = true;
                break;
            case "elevate_in":
                action = "elevate";
                reverse = false;
                break;
            case "extend_out":
                action = "extend";
                reverse = true;
                break;
            case "extend_in":
                action = "extend";
                reverse =  false;
                break;
        }

        if (!action)
            return;

        var url = this.robot_url + "event/" + action;

        fetch(url, {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                duration: 1.0,
                reverse: reverse
            })
        });
    }

    render() {
        return (
            <div className='single-action'>
                <button>
                    <span className="oi" onClick={() => this.doAction(this.props.actionName)} data-glyph={this.props.iconType}></span>
                </button>

                <label>
                    {this.props.actionLabel}
                </label>
            </div>
        );
    }
}

export default ActionsPanelSingleItemComponent;