import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';
import {testAction} from '../actions/actions';

import '../../scss/gamepad-component.scss'

class GamepadComponent extends Component {

    constructor(props) {
        super(props);

        this.is_moving = false;
        this.timeout_obj = undefined;
        this.timeout_milli = 1000;
        this.robot_url = "http://ML_DEVNET_PIBOT:9976/";

        this.getMvmtData = this.getMvmtData.bind(this);
        this.sendMvmtReq = this.sendMvmtReq.bind(this);
        this.moveRobot = this.moveRobot.bind(this);
        this.stopMoving = this.stopMoving.bind(this);
        this.haltRobot = this.haltRobot.bind(this);
    }

    getMvmtData(mvmt_type) {
        var obj = null;

        if (mvmt_type == "up" || mvmt_type == "down" || 
            mvmt_type == "left" || mvmt_type == "right" ||
            mvmt_type == "turn_left" || mvmt_type == "turn_right")
        {
            obj = {};
        }
        else
        {
            return obj;
        }

        if (mvmt_type == "up" || mvmt_type == "down")
        {
            obj.mvmt_name = "walk";
        }

        if (mvmt_type == "left" || mvmt_type == "right")
        {
            obj.mvmt_name = "strafe";
        }

        if (mvmt_type == "turn_left" || mvmt_type == "turn_right")
        {
            obj.mvmt_name = "turn";
        }

        if (mvmt_type == "up" || mvmt_type == "left" || mvmt_type == "turn_right")
        {
            obj.reverse = false;
        }
        else
        {
            obj.reverse = true;
        }

        if (mvmt_type == "turn_left" || mvmt_type == "turn_right")
        {
            obj.cycles = 5;
            obj.duration = 0.35;
            obj.delay = obj.duration * 5;
        }
        else
        {
            obj.cycles = 1;
            obj.duration = 1.0;
            obj.delay = obj.duration;
        }

        return obj;
    }

    sendMvmtReq(mvmt_type) {
        var mvmt_data = this.getMvmtData(mvmt_type);
        console.log(mvmt_data);

        var url = this.robot_url + "event/" + mvmt_data.mvmt_name;

        fetch(url, {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                cycles: mvmt_data.cycles,
                duration: mvmt_data.duration,
                reverse: mvmt_data.reverse
            })
        });

        return mvmt_data.delay * 1000;
    }

    moveRobot(mvmt_type) {
        var delay;
        delay = this.sendMvmtReq(mvmt_type);
        this.timeout_obj = setTimeout(() => this.moveRobot(mvmt_type), delay);
    }

    stopMoving() {
        var t = clearTimeout(this.timeout_obj);
    }

    haltRobot() {
        fetch('http://ML_DEVNET_PIBOT:9976/event/halt', {
            method: 'POST'
        });
    }

    render() {
        return (
                <section className="gamepad-component">
                    <div className="d-pad">
                        <button className='up' 
                            onMouseEnter={() => this.moveRobot("up")} 
                            onMouseLeave={() => this.stopMoving()}
                        >
                            <span className="oi mega" data-glyph="arrow-thick-top"></span>
                        </button>

                        <button className='stop' 
                            onMouseEnter={() => this.haltRobot()} 
                        >
                            <span className="oi mega" data-glyph="target"></span>
                        </button>

                        <button className='down' 
                            onMouseEnter={() => this.moveRobot("down")} 
                            onMouseLeave={() => this.stopMoving()}
                        >
                            <span className="oi mega" data-glyph="arrow-thick-bottom"></span>
                        </button>

                        <button className='right' 
                            onMouseEnter={() => this.moveRobot("right")} 
                            onMouseLeave={() => this.stopMoving()}
                        >
                            <span className="oi mega" data-glyph="arrow-thick-right"></span>
                        </button>

                        <button className='left' 
                            onMouseEnter={() => this.moveRobot("left")} 
                            onMouseLeave={() => this.stopMoving()}
                        >
                            <span className="oi mega" data-glyph="arrow-thick-left"></span>
                        </button>
                    </div>

                    <div className="buttons-panel">
                        <div className="distance">
                            <label>
                                Distance
                            </label>
                            <div className="data-val">
                                123.51cm
                            </div>
                        </div>

                        <div className="speed">
                            <label>
                                Speed
                            </label>
                            <input type="range" name="speed" />
                        </div>                           

                        <div className="rotate-buttons">
                            <button className='left' 
                                onMouseEnter={() => this.moveRobot("turn_left")} 
                                onMouseLeave={() => this.stopMoving()}
                            >
                                <span className="oi mega" data-glyph="action-undo"></span>
                            </button>

                            <button className='right' 
                                onMouseEnter={() => this.moveRobot("turn_right")} 
                                onMouseLeave={() => this.stopMoving()}
                            >
                                <span className="oi mega" data-glyph="action-redo"></span>
                            </button>                            
                        </div>
                    </div>
                </section>
            );
    }
}

function mapStateToProps(state) {
    if (!state.testReducer || !state.testReducer.first_obj) return {};
    return {
        first_obj: state.testReducer.first_obj
    }
}

function matchDispatchToProps(dispatch) {
    return bindActionCreators({
        testAction: testAction
    }, dispatch);
}

export default connect(mapStateToProps, matchDispatchToProps)(GamepadComponent);