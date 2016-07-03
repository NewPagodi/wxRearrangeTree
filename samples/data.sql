
CREATE TABLE Tree(
    ItemId   INTEGER PRIMARY KEY,
    Parent   INTEGER,
    Ordering INTEGER,
    AuxId    INTEGER
);

CREATE TABLE Data(
    AuxId INTEGER PRIMARY KEY,
    Label TEXT,
    Icon INTEGER
);

CREATE TABLE Expanded(
    ItemID INTEGER
);

CREATE TABLE SimpleTree(
    ItemId   INTEGER PRIMARY KEY,
    Parent   INTEGER,
    Ordering INTEGER,
    Label    TEXT,
    Icon     INTEGER,
    AuxId    INTEGER
);


select itemid,Label,parent,ordering from tree inner join data using (AuxId);

INSERT INTO Tree(ItemId,Parent,Ordering,AuxId) VALUES (null,0,1,0);

INSERT INTO Tree(ItemId,Parent,Ordering,AuxId) VALUES (null,1,1,1);
INSERT INTO Tree(ItemId,Parent,Ordering,AuxId) VALUES (null,1,2,2);
INSERT INTO Tree(ItemId,Parent,Ordering,AuxId) VALUES (null,1,3,3);

INSERT INTO Tree(ItemId,Parent,Ordering,AuxId) VALUES (null,2,1,4);
INSERT INTO Tree(ItemId,Parent,Ordering,AuxId) VALUES (null,2,2,5);

INSERT INTO Tree(ItemId,Parent,Ordering,AuxId) VALUES (null,3,1,6);
INSERT INTO Tree(ItemId,Parent,Ordering,AuxId) VALUES (null,3,2,7);

INSERT INTO Data(AuxId,Label,Icon) VALUES (0,'Root',-1);
INSERT INTO Data(AuxId,Label,Icon) VALUES (1,'Item 1',-1);
INSERT INTO Data(AuxId,Label,Icon) VALUES (2,'Item 2',-1);
INSERT INTO Data(AuxId,Label,Icon) VALUES (3,'Item 3',-1);
INSERT INTO Data(AuxId,Label,Icon) VALUES (4,'Item 1-1',-1);
INSERT INTO Data(AuxId,Label,Icon) VALUES (5,'Item 1-2',-1);
INSERT INTO Data(AuxId,Label,Icon) VALUES (6,'Item 2-1',-1);
INSERT INTO Data(AuxId,Label,Icon) VALUES (7,'Item 2-2',-1);

INSERT INTO SimpleTree(ItemId,Parent,Ordering,Label,Icon,AuxId) VALUES (null,0,1,"Root",-1,0);

INSERT INTO SimpleTree(ItemId,Parent,Ordering,Label,Icon,AuxId) VALUES (null,1,1,"Simple Item 1",0,0);
INSERT INTO SimpleTree(ItemId,Parent,Ordering,Label,Icon,AuxId) VALUES (null,1,2,"Simple Item 2",0,0);
INSERT INTO SimpleTree(ItemId,Parent,Ordering,Label,Icon,AuxId) VALUES (null,1,3,"Simple Item 3",0,0);

INSERT INTO SimpleTree(ItemId,Parent,Ordering,Label,Icon,AuxId) VALUES (null,2,1,"Simple Item 1-1",1,0);
INSERT INTO SimpleTree(ItemId,Parent,Ordering,Label,Icon,AuxId) VALUES (null,2,2,"Simple Item 1-2",1,0);

INSERT INTO SimpleTree(ItemId,Parent,Ordering,Label,Icon,AuxId) VALUES (null,3,1,"Simple Item 2-1",1,0);
INSERT INTO SimpleTree(ItemId,Parent,Ordering,Label,Icon,AuxId) VALUES (null,3,2,"Simple Item 2-2",1,0);

