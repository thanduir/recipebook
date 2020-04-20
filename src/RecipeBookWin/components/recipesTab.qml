import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {

    // Recipe dialogs

    TextInputDialog {
        id: dlgAddRecipe
        title: qsTr("Add recipe")
        onCurrentTextChanged: currentTextAllowed = !modelRecipes.existsRecipe(outputText)
        onAccepted: {
            lvRecipes.currentIndex = modelRecipes.addRecipe(outputText)
            lvRecipes.positionViewAtIndex(lvRecipes.currentIndex, ListView.Center)
        }
    }

    TextInputDialog {
        id: dlgCopyRecipe
        title: qsTr("Copy recipe")
        onCurrentTextChanged: currentTextAllowed = !modelRecipes.existsRecipe(outputText)
        onAccepted: {
            lvRecipes.currentIndex = modelRecipes.copyRecipe(lvRecipes.currentIndex, outputText)
            lvRecipes.positionViewAtIndex(lvRecipes.currentIndex, ListView.Center)
        }
    }

    TextInputDialog {
        id: dlgRenameRecipe
        title: qsTr("Rename recipe")
        onCurrentTextChanged: currentTextAllowed = !modelRecipes.existsRecipe(outputText)
        onAccepted: {
            lvRecipes.currentIndex = modelRecipes.renameRecipe(lvRecipes.currentIndex, outputText)
            lvRecipes.positionViewAtIndex(lvRecipes.currentIndex, ListView.Center)
        }
    }
    
    TextMessageDialog {
        id: dlgRemoveRecipe
        title: qsTr("Remove recipe")
        onAccepted: {
            modelRecipes.removeRecipe(lvRecipes.currentIndex)
            lvRecipes.incrementCurrentIndex()
            lvRecipes.decrementCurrentIndex()
        }
    }

    // Recipe item dialog
    
    EditItemsListDialog {
        id: dlgEditRecipeItemsList
        title: qsTr("Edit ingredients list")

        onListChanged: {
            lvCurrentRecipe.currentIndex = -1;
            modelRecipeItems.modelReset();
        }
    }

    // Alternatives groups

    EditAlternativesGroupDialog {
        id: editAlternativesGroupDialog
        title: qsTr("Edit alternatives groups")
    }

    // Recipes list

    Label {
        id: labelRecipes
        
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 48
        anchors.leftMargin: 48
        
        text: qsTr("Recipes")
        font.bold: true
    }

    ScrollView {
        id: scrollViewRecipes
        anchors.left: parent.left
        anchors.top: labelRecipes.bottom
        anchors.bottom: paneRecipes.top
        anchors.topMargin: 48
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        width: 300

        ListView {
            id: lvRecipes
            anchors.fill: parent
            topMargin: 5
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            boundsBehavior: Flickable.StopAtBounds

            spacing: 5
            model: modelRecipes
            delegate: ItemDelegate {
                highlighted: ListView.isCurrentItem
                onClicked: {
                    forceActiveFocus()
                    lvRecipes.currentIndex = index
                    modelRecipeItems.setRecipe(index)
                    lvCurrentRecipe.currentIndex = -1
                }
                width: lvRecipes.width - lvRecipes.leftMargin - lvRecipes.rightMargin
                
                text: isEverythingSet ? name : name + " (*)"
            }

            Component.onCompleted: {
                if(lvRecipes.count > 0) {
                    lvRecipes.currentIndex = 0;
                     modelRecipeItems.setRecipe(0);
                }
            }
        }
    }

    Pane {
        id: paneRecipes
        anchors.bottom: parent.bottom
        anchors.left: scrollViewRecipes.left
        anchors.right: scrollViewRecipes.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        RowLayout {
            anchors.fill: parent
        
            Button {
                id: buttonAdd

                text: qsTr("Add")
                onClicked: dlgAddRecipe.open()
            }

            Button {
                id: buttonCopy

                text: qsTr("Copy")
                onClicked: {
                    dlgCopyRecipe.initialText = modelRecipes.name(lvRecipes.currentIndex);
                    dlgCopyRecipe.open();
                }
            }

            Button {
                id: buttonRename

                text: qsTr("Rename")
                onClicked: {
                    dlgRenameRecipe.initialText = modelRecipes.name(lvRecipes.currentIndex);
                    dlgRenameRecipe.open();
                }
            }

            Button {
                id: buttonRemove

                text: qsTr("Remove")
                enabled: lvRecipes.currentIndex < lvRecipes.count && lvRecipes.currentIndex >= 0
                onClicked: {
                    dlgRemoveRecipe.msgText = qsTr("This will remove the recipe \"" + modelRecipes.name(lvRecipes.currentIndex) + "\". Proceed?");
                    dlgRemoveRecipe.open();
                }
            }
        }
    }

    Label {
        id: labelCurrentRecipe
        
        anchors.left: grid.left
        anchors.top: parent.top
        anchors.topMargin: 48
        
        text: qsTr("Recipe \"" + modelRecipes.name(lvRecipes.currentIndex) + "\"")
        font.bold: true
    }

    GridLayout {
        id: grid
        anchors.left: scrollViewRecipes.right
        anchors.top: labelCurrentRecipe.bottom
        anchors.topMargin: 24
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        anchors.rightMargin: 24
        
        width: 375

        visible: lvRecipes.count > 0
        columns: 2

        Label { 
            text: qsTr("Number of persons") 
        }
        SpinBox { 
            from: 1
            to: 50
            editable: true
            wheelEnabled: true
            value: modelRecipes.numberOfPersons(lvRecipes.currentIndex)

            onValueModified: modelRecipes.setNumberOfPersons(lvRecipes.currentIndex, value)
        }

        Label { 
            Layout.rightMargin: 50
            text: qsTr("Cooking time")
        }
        SpinBox { 
            from: 1
            to: 359
            editable: true
            wheelEnabled: true
            inputMethodHints: Qt.ImhTime
            validator: RegularExpressionValidator { regularExpression: /^([0][0-5]):([0-5][0-9])$/ }
            textFromValue: function(value, locale) {
                var hour = Math.floor(value / 60);
                var min = value % 60;
                var hourStr = hour < 10 ? "0" + hour : hour;
                var minStr = min < 10 ? "0" + min : min;
                return hourStr + ":" + minStr;
            }
            valueFromText: function(text, locale) {
                var cookingTime = new Date()
                cookingTime = Date.fromLocaleString(locale, text, "HH:mm");
                return cookingTime.getHours() * 60 + cookingTime.getMinutes();
            }
            value: modelRecipes.cookingTime(lvRecipes.currentIndex)
                        
            onValueModified: modelRecipes.setCookingTime(lvRecipes.currentIndex, value)

            ToolTip.delay: 1000
            ToolTip.timeout: 5000
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Format: \"HH:mm\"")
        }
    }

    TextField { 
        id: textShortDesc
        anchors.top: grid.bottom
        anchors.left: scrollViewRecipes.right
        anchors.right: grid.right
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        anchors.rightMargin: 24

        selectByMouse: true

        ToolTip.delay: 1000
        ToolTip.timeout: 3000
        ToolTip.visible: hovered
        ToolTip.text: qsTr("Short description")

        text: modelRecipes.shortDescription(lvRecipes.currentIndex)
        onEditingFinished: modelRecipes.setShortDescription(lvRecipes.currentIndex, text)
    }

    ScrollView {
        anchors.top: textShortDesc.bottom
        anchors.left: scrollViewRecipes.right
        anchors.right: grid.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        anchors.rightMargin: 24
        
        TextArea { 
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: 375

            wrapMode: TextEdit.Wrap
            selectByMouse: true

            ToolTip.delay: 1000
            ToolTip.timeout: 3000
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Recipe text")

            text: modelRecipes.recipeText(lvRecipes.currentIndex)
            onEditingFinished: modelRecipes.setRecipeText(lvRecipes.currentIndex, text)
        }
    }

    ScrollView {
        id: scrollViewCurrentRecipe
        anchors.left: grid.right
        anchors.top: labelCurrentRecipe.bottom
        anchors.bottom: paneCurrentRecipe.top
        anchors.topMargin: 48
        anchors.leftMargin: 24
        anchors.bottomMargin: 48
        width: 400

        ListView {
            id: lvCurrentRecipe
            anchors.fill: parent
            topMargin: 5
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            boundsBehavior: Flickable.StopAtBounds
            keyNavigationEnabled: false
            
            spacing: 5
            model: modelRecipeItems
            delegate: ItemDelegate {
                id: listItemRecipeItem
                highlighted: ListView.isCurrentItem
                onPressed: lvCurrentRecipe.currentIndex == index ? lvCurrentRecipe.currentIndex = -1 : lvCurrentRecipe.currentIndex = index
                width: lvCurrentRecipe.width - lvCurrentRecipe.leftMargin - lvCurrentRecipe.rightMargin
                height: listItemRecipeItemGroup.height

                Item {
                    id: listItemRecipeItemGroup
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 10

                    height: listItemRecipeItemName.height + 30 + (highlighted ? listItemGridRecipeItem.height : 0)

                    // Ingredient / group name
                    Label {
                        id: listItemRecipeItemName
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        
                        font.bold: !optional
                        font.italic: optional
                        text: name
                        verticalAlignment: Text.AlignVCenter
                    }

                    // Summary for inactive ingredients
                    Label {
                        function recipeItemSmallDesc(n){
                            var text = "";
                            if(group != alternativesGroups.stringNoAlternativesGroup())
                            {
                                text += group;
                            }
                            if(amountUnit != modelRecipeItems.indexUnitUnitless())
                            {
                                if(text != "")
                                {
                                    text = text + ", ";
                                }
                                text += amountMin;
                                if(amountIsRange)
                                {
                                    text += "-" + amountMax;
                                }
                                text += " " + unitNamesShort[amountUnit];
                            }
                            if(sizeIndex != 1)
                            {
                                if(text != "")
                                {
                                    text = text + ", ";
                                }
                                text += sizeNames[sizeIndex]
                            }
                            if(additionalInfo != "")
                            {
                                if(text != "")
                                {
                                    text = text + ", ";
                                }
                                text += additionalInfo;
                            }

                            if(text != "")
                            {
                                text = " (" + text + ")";
                            }
                            return text;
                        }

                        anchors.left: listItemRecipeItemName.right
                        color: "gray"
                        text: recipeItemSmallDesc(index)
                        verticalAlignment: Text.AlignVCenter
                        font.italic: optional
                        width: parent.width - listItemRecipeItemName.width - 10
                        wrapMode: Label.WordWrap
                        visible: !listItemRecipeItem.highlighted
                    }

                    // Group symbol
                    Rectangle {
                        id: rowGroupHeaderButtons
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.rightMargin: 10
                        width: 10
                        height: width
                        radius: 0.5 * width

                        color: groupColor
                        visible: group != alternativesGroups.stringNoAlternativesGroup()
                    }

                    // Extended information for active ingredient elements
                    GridLayout {
                        id: listItemGridRecipeItem
                        visible: listItemRecipeItem.highlighted

                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: listItemRecipeItemName.bottom
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.topMargin: 10
                        
                        columns: 2
                        columnSpacing: 10
                        
                        Label { 
                            text: qsTr("Amount")
                        }
                        RowLayout { 
                            Layout.fillWidth: true

                            ComboBox {
                                Layout.fillWidth: true
                                model: unitNames
                                currentIndex: amountUnit
                                onActivated: amountUnit = currentIndex
                            }

                            CheckBox {
                                visible: amountUnit != modelRecipeItems.indexUnitUnitless()

                                text: qsTr("Range")
                                checked: amountIsRange

                                onClicked: amountIsRange = checked
                            }
                        }

                        Label { 
                            visible: amountUnit != modelRecipeItems.indexUnitUnitless()
                            text: " "
                        }
                        RowLayout {
                            visible: amountUnit != modelRecipeItems.indexUnitUnitless()
                            spacing: 20

                            Label { 
                                visible: amountIsRange
                                text: qsTr("Min.")
                            }
                            TextField { 
                                Layout.preferredWidth: 75
                                selectByMouse: true
                                horizontalAlignment: TextInput.AlignRight
                                onFocusChanged: {
                                    if(focus)
                                        selectAll()
                                }

                                text: amountMin
                                validator: DoubleValidator { bottom: 0; top: 9999; decimals: 3; locale: "en_US" }
                                onEditingFinished: {
                                    if(amountIsRange && text > amountMax)
                                    {
                                        text = amountMax;
                                    }
                                    amountMin = text;
                                }
                            }

                            Label {
                                visible: amountIsRange
                                text: qsTr("Max.") 
                            }
                            TextField { 
                                Layout.preferredWidth: 75
                                visible: amountIsRange
                                
                                selectByMouse: true
                                horizontalAlignment: TextInput.AlignRight
                                onFocusChanged: {
                                    if(focus)
                                        selectAll()
                                }

                                validator: DoubleValidator { bottom: 0; top: 9999; decimals: 3; locale: "en_US" }
                                text: amountMax
                                onEditingFinished: {
                                    if(amountIsRange && text < amountMin)
                                    {
                                        text = amountMin;
                                    }
                                    amountMax = text
                                }
                            }
                        }

                        Label { 
                            text: qsTr("Size") 
                        }
                        SpinBox { 
                            from: 0
                            to: sizeNames.length - 1
                            
                            textFromValue: function(value, locale) {
                                return sizeNames[value];
                            }
                            valueFromText: function(text, locale) {
                                return sizeNames.indexOf(text)
                            }

                            value: sizeIndex
                            onValueModified: sizeIndex = value
                        }

                        Label { 
                            text: qsTr("Add. Info") 
                        }
                        TextField { 
                            Layout.fillWidth: true
                            selectByMouse: true

                            text: additionalInfo
                            onEditingFinished: additionalInfo = text
                        }

                        Label { 
                            text: " "
                        }
                        CheckBox {
                            text: qsTr("Optional")
                            checked: optional

                            onClicked: optional = checked
                        }

                        Label { text: qsTr("Group") }
                        ComboBox {
                            Layout.fillWidth: true
                            
                            model: alternativesGroups
                            textRole: "name"
                            valueRole: "name"

                            currentIndex: indexOfValue(group)
                            onActivated: group = currentText

                            ToolTip.delay: 1000
                            ToolTip.timeout: 5000
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Alternatives group")                        

                            onVisibleChanged: {
                                if(visible)
                                    currentIndex = indexOfValue(group)
                            }
                        }
                    }
                }
            }
        }
    }

    Pane {
        id: paneCurrentRecipe
        anchors.left: scrollViewCurrentRecipe.left
        anchors.right: scrollViewCurrentRecipe.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 48

        RowLayout {
            anchors.fill: parent
        
            Button {
                //anchors.right: buttonEditAlternativesGroup.left

                text: qsTr("Edit list") 
                //icon.source: "qrc:/images/edit.svg"

                onClicked: {
                    dlgEditRecipeItemsList.editListModel = modelRecipeItems;
                    dlgEditRecipeItemsList.allValuesFilterModel = filterModelIngredients;
                    dlgEditRecipeItemsList.initialyHighlightedIndex = modelIngredients.indexOfIngredient(modelRecipeItems.name(lvCurrentRecipe.currentIndex))

                    dlgEditRecipeItemsList.open();
                }
            }

            Button {
                id: buttonEditAlternativesGroup
                //anchors.right: parent.right

                text: qsTr("Edit alternatives groups") 
                icon.source: "qrc:/images/edit.svg"

                onClicked: {
                    editAlternativesGroupDialog.open();
                }
            }
        }
    }
}