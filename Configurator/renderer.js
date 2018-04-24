const SerialPort = require('serialport');
const remote = require('electron').remote;

// For todays date;
Date.prototype.today = function () {
    return this.getFullYear() + "-" + (((this.getMonth() + 1) < 10) ? "0" : "") + (this.getMonth() + 1) + "-" + ((this.getDate() < 10) ? "0" : "") + this.getDate();
};

// For the time now
Date.prototype.timeNow = function () {
    return ((this.getHours() < 10) ? "0" : "") + this.getHours() + ":" + ((this.getMinutes() < 10) ? "0" : "") + this.getMinutes() + ":" + ((this.getSeconds() < 10) ? "0" : "") + this.getSeconds();
};

function addLeadingZeros(number, length=2){
    var my_string = '' + number;
    while (my_string.length < length) {
        my_string = '0' + my_string;
    }

    return my_string;
}

let drinkBuddy = undefined;

function saveConfig() {
    if (drinkBuddy == undefined) return;

    drinkBuddy.write("NAME " + $("#name").val() + "\n");
    drinkBuddy.write("BRIGHTNESS " + $("#brightness").val() + "\n");
    drinkBuddy.write("MINIMUMLITRE " + $("#minimuml").val() + "\n");
    drinkBuddy.write("TARGETLITRE " + $("#targetl").val() + "\n");
    drinkBuddy.write("STARTTIME " + addLeadingZeros($("#startHour").val()) + ":" + addLeadingZeros($("#startMinute").val()) + "\n");
    console.log("STARTTIME " + addLeadingZeros($("#startHour").val()) + ":" + addLeadingZeros($("#startMinute").val()) + "\n");
    drinkBuddy.write("ENDTIME " + addLeadingZeros($("#endHour").val()) + ":" + addLeadingZeros($("#endMinute").val()) + "\n");

    window.setTimeout(function () {
        if (!$("#enableReminder").is(":checked")) {
            drinkBuddy.write("REMINDERMINUTES -1 \n");
        } else {
            drinkBuddy.write("REMINDERMINUTES " + $("#remindtime").val() + "\n");
        }
    }, 500);

    window.setTimeout(function () {
        window.setTimeout(function () {
            drinkBuddy.write("CONFIG FINISH\n");
        }, 500);
        window.setTimeout(function () {
            drinkBuddy.close();
            $("#form").hide();
        }, 1000);
    }, 2000);


}

function findDrinkBuddy() {
    $("#form").hide();
    $("#info").hide();
    $("#searching").show();
    $("#findDrinkBuddy").prop("disabled", true);
    $("#remindtime").prop("disabled", true);

    SerialPort.list((err, ports) => {
        let found = false;
        ports.forEach(function (port) {
            if (!found && port.productId == "0x7523") {
                drinkBuddy = new SerialPort(port.comName, {
                    baudRate: 19200,
                    parser: SerialPort.parsers.readline('\n')
                });

                if (drinkBuddy.isOpen()) {
                    drinkBuddy.close();
                }

                drinkBuddy.on('error', function (err) {
                    console.error('Error: ', err.message);
                    $("#errorWrapper").show();
                    $("#error").html(err.message);
                });

                drinkBuddy.on('close', function () {
                    drinkBuddy = undefined;
                    $("#form").hide();
                    $("#info").show();
                    $("#searching").hide();
                });

                drinkBuddy.on('data', function (data) {
                    console.log(data);

                    if (data.trim() === "DrinkBuddy") {
                        found = true;
                    }

                    if (data.startsWith(";NAME")) {
                        let config = {};
                        data.split(";").forEach(function (entry) {
                            if (entry.trim() === "") return;
                            let tmp = entry.split("=");
                            config[tmp[0]] = tmp[1];
                        });

                        $("#name").val(config.NAME);
                        $("#version").val(config.VERSION);
                        let start = config.STARTTIME.split(":");
                        $("#startHour").val(start[0]);
                        $("#startMinute").val(start[1]);
                        let end = config.ENDTIME.split(":");
                        $("#endHour").val(end[0]);
                        $("#endMinute").val(end[1]);
                        $("#minimuml").val(config.MINIMUMLITRE);
                        $("#dynmin").val(config.MINLDYN);
                        $("#targetl").val(config.TARGETLITRE);
                        $("#brightness").val(config.BRIGHT);

                        if (config.RMNDRMNTS > 0) {
                            $("#enableReminder").prop("checked", true);
                            $("#remindtime").prop("disabled", false).val(config.RMNDRMNTS);

                        } else {
                            $("#enableReminder").prop("checked", false);
                            $("#remindtime").prop("disabled", true);
                        }

                        $("#form").show();
                        $("#searching").hide();
                        $("#findDrinkBuddy").prop("disabled", false);

                        drinkBuddy.write("CONFIG START\n");
                    }

                    if (data.startsWith("CONFIG MODE ENABLED")) {
                        let timestamp = new Date();
                        let time = timestamp.timeNow();
                        let date = timestamp.today();
                        drinkBuddy.write("TIME " + time);
                        drinkBuddy.write("DATE " + date);
                    }
                });
            }
        });

        if (err) {
            $("#error").html(err.message);
            $("#errorWrapper").show();
            return
        } else {
            $("#error").html("");
            $("#errorWrapper").hide();
        }

        if (ports.length === 0) {
            $("#error").html("No ports discovered");
            $("#errorWrapper").show();
        }
    });
}

$(document).ready(function () {
    $("#searching").hide();
    $("#form").hide();
    $("#info").show();
    $("#findDrinkBuddy").click(function (e) {
        e.preventDefault();
        findDrinkBuddy();
    });

    $("#saveConfig").click(() => {
        saveConfig();
    });

    $("#enableReminder").change(function () {
        if ($(this).is(":checked")) {
            $("#remindtime").prop("disabled", false).val(30);
            return;
        }
        $("#remindtime").prop("disabled", true);
    });
});
