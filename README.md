## wxRearrangeTree
This is a subclass of wxTreeCtrl that adds a number of methods to allow the nodes to be rearranged and allow the structure to be preserved in a wxsqlite3 database.  

### Basic Usage
To use the control, construct it as you would any other tree control and then call the method:

```C++
SetHelperAndData(wxRearrangeTreeHelper* h, wxSQLite3Database* d, const wxString& t, const wxString& e=wxEmptyString);
```
where h is a special helper class that will be described later, d is a pointer to a wxsqlite3 database, t is the name of the table in the database describing a tree, and e is an optional table tracking the expanded nodes in the table.

The database table describing the tree is assumed to look something like the following:

```SQL
CREATE TABLE Tree(
    ItemId   INTEGER PRIMARY KEY,
    Parent   INTEGER,
    Ordering INTEGER,
    AuxId    INTEGER
);
```
where ItemId is generated automatically by the control, Parent is the item id of the node's parent, Ordering arranges the nodes in their parent, and AuxId is an auxiliary piece of data that can be used to tie the node to other sources of data.

A node whose parent is 0 is assumed to be a root node.  Each table should have 1 and only 1 root node.  If there is no root node, the control will be unable to construct a tree.  If there is more than 1 root node, the results should be considered unpredictable.

### wxRearrangeTreeHelper
The table described above only specifies the topology of the tree.  It does not give labels or icons to the nodes of the tree.  Since the way that is done is probably going to be different for every database, that task is done by a second class - wxRearrangeTreeHelper.  This class is specified as:

```C++
class wxRearrangeTreeHelper
{
    public:
        wxRearrangeTreeHelper(){}
        virtual ~wxRearrangeTreeHelper(){}
        virtual wxString GetLabel(int i,int d){return wxString::Format("Item (%d,%d)",i,d);}
        virtual int GetIcon(int,int,wxTreeItemIcon){return -1;}
        virtual void UpdateLabel(int,int,const wxString&){}
        virtual void UpdateIcon(int,int,int,wxTreeItemIcon){}
        virtual bool IsOk(){return true;}
};
```
To use this in your application, you would derive a class from wxRearrangeTreeHelper and override the virtual methods to return the labels and icons based on the ItemId and AuxId of the node.  You can also override UpdateLabel and UpdateIcon to handle update the database when the labels or icons are changed by the control.

### Remembering the Expanded Nodes
The expanded nodes can be remembered by passing the name of a database table to receive them as the last parameter in the SetHelperAndData call.  This table should simply look like the following:

```SQL
CREATE TABLE Expanded(
    ItemID INTEGER
);
```

### wxSimpleRearrangeTree
If all you want to do is have a persistant tree that is not necessarily connected to other data, a special class, wxSimpleRearrangeTree, is also available.  This class uses its own helper and assumes the database table looks like the following:

```SQL
CREATE TABLE SimpleTree(
    ItemId   INTEGER PRIMARY KEY,
    Parent   INTEGER,
    Ordering INTEGER,
    Label    TEXT,
    Icon     INTEGER,
    AuxId    INTEGER
);
```

### Notes
In order to preserve the sync between the database and the control, the following methods were made private in this subclass:
* wxTreeCtrl::AddRoot;
* wxTreeCtrl::AppendItem;
* wxTreeCtrl::InsertItem;
* wxTreeCtrl::PrependItem;
* wxTreeCtrl::SetItemData;
New methods for adding nodes were added to replace these methods.

### Requirements
* wxWidgets (3.0 or later)
* wxsqlite3 (I think any version will work)

### License
wxWindows Library Licence, Version 3.1
