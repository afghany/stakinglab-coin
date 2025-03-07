#include "devfee.h"
#include "util.h"
#include "main.h"

DevFee::DevFee()
{
   address = CBitcoinAddress("LPL2FGeRM21VUZWXRkGHngRpEHf7Q1VJBd");
}

void DevFee::Create(CMutableTransaction& txNew, const int idxProofOfStake, const int nBlockHeight)
{
    CAmount nReward = GetBlockValue(nBlockHeight);

    int i = txNew.vout.size();
    CAmount nAmount = nReward * fPercentage;

    LogPrint("devfee", "DevFee::Create() vout size: %s, nReward=%d, nAmount=%d\n", i, nReward, nAmount);

    txNew.vout.resize(i + 1);
    txNew.vout[idxProofOfStake].nValue -= nAmount;
    txNew.vout[i].scriptPubKey = GetScriptForDestination(address.Get());
    txNew.vout[i].nValue = nAmount;
}

bool DevFee::Validate(const CBlock& block, const int nBlockHeight)
{
    CAmount nReward = GetBlockValue(nBlockHeight);

    int i = block.vtx[1].vout.size();
    CAmount nExpectedAmount = nReward * fPercentage;

    LogPrint("devfee", "DevFee::Validate() vout size: %s, nReward=%d, nExpectedAmount=%d\n", i, nReward, nExpectedAmount);

    if (block.vtx[1].vout[i - 1].scriptPubKey != GetScriptForDestination(address.Get())) {
        LogPrintf("DevFee::Validate() : ERROR invalid scriptPubKey\n");
        return false;
    }



    return true;
}
